// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

#include <libversa/Result.h>
#include <libversa/Logger.h>
#include <libversa/Object.h>

namespace libversa {

class Repo {
private:
    Logger& logger;
    std::string currentRepo;
    Result<std::filesystem::path> make_object_path(Object& obj);
public:
    Repo(Logger& logger);
    Result<bool> init();
    Result<bool> open_repo(std::string directory);
    Result<bool> hash_object(Object& obj, bool writeOut);
};

};