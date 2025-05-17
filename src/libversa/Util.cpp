// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Util.h>

using namespace libversa;

Result<std::vector<uint8_t>> Util::read_file(std::string path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) {
        return Result<std::vector<uint8_t>>::failure("Error opening file '" + path + "'");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return Result<std::vector<uint8_t>>::failure("Error reading file '" + path + "'");
    }

    return Result<std::vector<uint8_t>>::success(buffer);
}

std::string Util::hash_to_string(Blake3Hash hash) {
    std::ostringstream oss;
    for (uint8_t byte: hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}