// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <zstd/lib/zstd.h>

#include <vector>
#include <cstdint>

#include <libversa/Result.h>

namespace libversa {

class Compress {
public:
    static Result<std::vector<uint8_t>> compress_bytes(std::vector<uint8_t> bytes);
    static Result<std::vector<uint8_t>> decompress_bytes(std::vector<uint8_t> bytes);
};

};