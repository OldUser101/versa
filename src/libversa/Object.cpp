// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Object.h>

#include <libversa/Hash.h>
#include <libversa/Util.h>
#include <libversa/Compress.h>

using namespace libversa;

ObjectType Object::get_type() {
    return this->type;
}

Blake3Hash Object::hash_blake3() {
    return Hash::hash_object_blake3(*this);
}

std::vector<uint8_t> Object::serialize() {
    std::string headerType;

    switch(this->type) {
        case BLOB:
            headerType = "blob ";
            break;
        case COMMIT:
            headerType = "commit ";
            break;
        case TREE:
            headerType = "tree ";
            break;
        case VIEW:
            headerType = "tag ";
            break;
    }

    std::string header = headerType + std::to_string(this->data.size()) + '\0';

    std::vector<uint8_t> result;
    result.reserve(header.size() + data.size());

    result.insert(result.end(), header.begin(), header.end());
    result.insert(result.end(), this->data.begin(), this->data.end());

    return result;
}

Result<std::unique_ptr<ObjectContent>> Object::deserialize() {
    auto nullPos = std::find(this->data.begin(), this->data.end(), '\0');
    if (nullPos == this->data.end()) {
        return Result<std::unique_ptr<ObjectContent>>::failure("Invalid or corrupt object data.");
    }

    std::string header(this->data.begin(), nullPos);
    std::istringstream headerStream(header);
    std::string typeStr;
    size_t size;

    headerStream >> typeStr >> size;
    auto contentStart = nullPos + 1;

    if (data.end() - contentStart != static_cast<ptrdiff_t>(size)) {
        return Result<std::unique_ptr<ObjectContent>>::failure("Invalid or corrupt object data.");
    }

    std::vector<uint8_t> content(contentStart, data.end());

    if (typeStr == "blob" && this->type == BLOB) {
        return Result<std::unique_ptr<ObjectContent>>::success(std::make_unique<Blob>(content));
    }

    return Result<std::unique_ptr<ObjectContent>>::failure("Invalid or corrupt object data.");
}

Object Blob::to_object() const {
    return Object(this->type, this->data);
}

void Blob::print() const {
    std::cout.write(reinterpret_cast<const char*>(this->data.data()), this->data.size());
    std::cout.flush();
}