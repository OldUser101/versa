// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>

#include <libversa/Result.h>

namespace libversa {

class Util {
public:
    static Result<std::vector<uint8_t>> read_file(std::string path);
};

};