// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <filesystem>

#include <libversa/Repo.h>
#include <libversa/Util.h>
#include <libversa/Compress.h>

#define VERSA_OBJ_PREFIX_LEN 2

using namespace libversa;

Repo::Repo(Logger& logger): logger(logger) {
    this->currentRepo = "";
}

Result<bool> Repo::init() {

    if (std::filesystem::exists("./.versa")) {
        return Result<bool>::failure("A VERSA repository already exists in this directory.");
    }

    if (!std::filesystem::create_directory("./.versa")) {
        return Result<bool>::failure("Failed to create directory '.versa'.");
    }

    if (!std::filesystem::create_directory("./.versa/objects")) {
        return Result<bool>::failure("Failed to create directory '.versa/objects'.");
    }

    this->logger.log("Created an empty VERSA repository in the current directory.", INFO);

    return Result<bool>::success(true);
}

Result<bool> Repo::open_repo(std::string directory) {
    std::filesystem::path repoDir = directory;
    repoDir /= ".versa";

    if (!std::filesystem::exists(repoDir)) {
        return Result<bool>::failure("A VERSA repository does not exist in '" + directory + "'.");
    }

    this->currentRepo = repoDir;
    return Result<bool>::success(true);
}

Result<bool> Repo::hash_object(Object& obj, bool writeOut) {
    if (this->currentRepo == "") {
        return Result<bool>::failure("No VERSA repository has been opened.");
    }

    Blake3Hash hash = obj.hash_blake3();
    this->logger.log(Util::hash_to_string(hash), NONE);

    if (!writeOut) {
        return Result<bool>::success(true);
    }

    Result<std::filesystem::path> rObjPath = this->make_object_path(obj);
    if (!rObjPath.ok()) {
        return Result<bool>::failure(rObjPath.msg);
    }

    std::filesystem::path objPath = rObjPath.get();
    std::vector<uint8_t> data = obj.serialize();

    Result<std::vector<uint8_t>> compressedDataResult = Compress::compress_bytes(data);

    if (!compressedDataResult.ok()) {
        return Result<bool>::failure(compressedDataResult.msg);
    }

    std::vector<uint8_t> compressedData = compressedDataResult.get();

    std::ofstream objectFile;
    objectFile.open(objPath, std::ios::binary | std::ios::out);

    if (!objectFile.is_open()) {
        return Result<bool>::failure("Error opening file '" + objPath.string() + "' in write mode.");
    }

    objectFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    if (!objectFile) {
        return Result<bool>::failure("Error writing file '" + objPath.string() + "'");
    }

    objectFile.close();

    return Result<bool>::success(true);
}

Result<std::filesystem::path> Repo::make_object_path(Object& obj) {
    Blake3Hash hash = obj.hash_blake3();
    std::string hashStr = Util::hash_to_string(hash);

    std::filesystem::path targetDir = this->currentRepo;
    targetDir /= "objects";
    targetDir /= hashStr.substr(0, VERSA_OBJ_PREFIX_LEN);

    if (!std::filesystem::exists(targetDir)) {
        if (!std::filesystem::create_directories(targetDir)) {
            return Result<std::filesystem::path>::failure("Error creating directory '" + targetDir.string() + "'.");
        }
    }

    return Result<std::filesystem::path>::success(targetDir / hashStr.substr(VERSA_OBJ_PREFIX_LEN));
}

Result<std::filesystem::path> Repo::get_object_path(std::string id) {
    std::filesystem::path target = this->currentRepo;
    target /= "objects";
    target /= id.substr(0, VERSA_OBJ_PREFIX_LEN);
    target /= id.substr(VERSA_OBJ_PREFIX_LEN);

    if (!std::filesystem::exists(target)) {
        return Result<std::filesystem::path>::failure("Invalid object ID '" + id + "'.");
    }

    return Result<std::filesystem::path>::success(target);
}

Result<std::unique_ptr<ObjectContent>> Repo::cat_file(ObjectType type, std::string objId) {
    if (this->currentRepo == "") {
        return Result<std::unique_ptr<ObjectContent>>::failure("No VERSA repository has been opened.");
    }
    
    Result<std::filesystem::path> rObjPath = this->get_object_path(objId);
    if (!rObjPath.ok()) {
        return Result<std::unique_ptr<ObjectContent>>::failure(rObjPath.msg);
    }

    std::filesystem::path objPath = rObjPath.get();

    std::ifstream objectFile;
    objectFile.open(objPath, std::ios::binary | std::ios::in);

    if (!objectFile.is_open()) {
        return Result<std::unique_ptr<ObjectContent>>::failure("Error opening file '" + objPath.string() + "' in read mode.");
    }

    size_t fSize = std::filesystem::file_size(objPath);

    std::vector<uint8_t> cBuf(fSize);
    objectFile.read(reinterpret_cast<char*>(cBuf.data()), cBuf.size());
    if (!objectFile) {
        return Result<std::unique_ptr<ObjectContent>>::failure("Error reading file '" + objPath.string() + "'");
    }

    Result<std::vector<uint8_t>> rDData = Compress::decompress_bytes(cBuf);
    if (!rDData.ok()) {
        return Result<std::unique_ptr<ObjectContent>>::failure(rDData.msg);
    }

    std::vector<uint8_t> dData = rDData.get();
    Object obj = Object(type, dData);
    
    Result<std::unique_ptr<ObjectContent>> rObjContent = obj.deserialize();
    if (!rObjContent.ok()) {
        return Result<std::unique_ptr<ObjectContent>>::failure(rObjContent.msg);
    }

    return Result<std::unique_ptr<ObjectContent>>::success(std::move(rObjContent.get()));
} 