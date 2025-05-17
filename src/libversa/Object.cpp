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

Object Blob::to_object() const {
    return Object(this->type, this->data);
}