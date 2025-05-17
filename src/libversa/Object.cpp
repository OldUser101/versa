// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Object.h>

#include <libversa/Hash.h>
#include <libversa/Util.h>
#include <libversa/Compress.h>

using namespace libversa;

ObjectType Object::get_type() const {
    return this->type;
}

Blake3Hash Object::hash_blake3() const {
    return Hash::hash_object_blake3(*this);
}

Result<std::string> Object::make_object_path() const {
    Blake3Hash hash = this->hash_blake3();
    std::string hashStr = Util::hash_to_string(hash);

    std::string targetDir = "./.versa/objects/" + hashStr.substr(0, 2);

    if (!std::filesystem::exists(targetDir)) {
        if (!std::filesystem::create_directories(targetDir)) {
            return Result<std::string>::failure("Error creating directory '" + targetDir + "'.");
        }
    }

    return Result<std::string>::success(targetDir + "/" + hashStr.substr(2));
}

Result<bool> Object::hash_object() const {
    Result<std::string> objectPathResult = this->make_object_path();

    if (!objectPathResult.ok()) {
        return Result<bool>::failure(objectPathResult.msg);
    }

    std::string objectPath = objectPathResult.value;
    std::vector<uint8_t> data = this->serialize();

    Result<std::vector<uint8_t>> compressedDataResult = Compress::compress_bytes(data);

    if (!compressedDataResult.ok()) {
        return Result<bool>::failure(compressedDataResult.msg);
    }

    std::vector<uint8_t> compressedData = compressedDataResult.value;

    std::ofstream objectFile;
    objectFile.open(objectPath, std::ios::binary | std::ios::out);

    if (!objectFile.is_open()) {
        return Result<bool>::failure("Error opening file '" + objectPath + "' in write mode.");
    }

    objectFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    if (!objectFile) {
        return Result<bool>::failure("Error writing file '" + objectPath + "'");
    }

    objectFile.close();

    return Result<bool>::success(true);
}

std::vector<uint8_t> Blob::serialize() const {
    std::string header = "blob " + std::to_string(this->data.size()) + '\0';

    std::vector<uint8_t> result;
    result.reserve(header.size() + data.size());

    result.insert(result.end(), header.begin(), header.end());

    result.insert(result.end(), this->data.begin(), this->data.end());

    return result;
}