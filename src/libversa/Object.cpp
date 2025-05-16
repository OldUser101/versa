// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Object.h>

#include <libversa/Hash.h>

using namespace libversa;

ObjectType Object::get_type() const {
    return this->type;
}

Blake3Hash Object::hash_blake3() const {
    return Hash::hash_object_blake3(*this);
}

std::vector<uint8_t> Blob::serialize() const {
    std::string header = "blob " + std::to_string(this->data.size()) + '\0';

    std::vector<uint8_t> result;
    result.reserve(header.size() + data.size());

    result.insert(result.end(), header.begin(), header.end());

    result.insert(result.end(), header.begin(), header.end());

    return result;
}