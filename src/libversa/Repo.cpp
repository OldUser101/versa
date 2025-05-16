// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <filesystem>

#include <libversa/Repo.h>

using namespace libversa;

Repo::Repo(Logger& logger): logger(logger) {}

Result<bool> Repo::init() {
    if (std::filesystem::exists("./.versa")) {
        return Result<bool>::failure("A VERSA repository already exists in this directory.");
    }

    if (!std::filesystem::create_directory("./.versa")) {
        return Result<bool>::failure("Failed to create directory '.versa'.");
    }

    if (!std::filesystem::create_directory("./.versa/objects")) {
        return Result<bool>::failure("Failed to create directory '.versa/objects'.");
    }

    this->logger.log("Created an empty VERSA repository in the current directory.", INFO);

    return Result<bool>::success(true);
}