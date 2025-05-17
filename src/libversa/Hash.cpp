// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Hash.h>

using namespace libversa;

Blake3Hash Hash::hash_object_blake3(Object& object) {
    std::vector<uint8_t> data = object.serialize();

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    blake3_hasher_update(&hasher, data.data(), data.size());

    Blake3Hash hash(BLAKE3_OUT_LEN);
    blake3_hasher_finalize(&hasher, hash.data(), hash.size());

    return hash;
}
