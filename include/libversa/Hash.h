// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <BLAKE3/c/blake3.h>

#include <libversa/Object.h>

namespace libversa {

using Blake3Hash = std::vector<uint8_t>;

class Hash {
public:
    static Blake3Hash hash_object_blake3(Object& object);
};

};