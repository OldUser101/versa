// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <libversa/Result.h>

namespace libversa {

using Blake3Hash = std::vector<uint8_t>;

class Util {
public:
    static Result<std::vector<uint8_t>> read_file(std::string path);
    static std::string hash_to_string(Blake3Hash hash);
};

};