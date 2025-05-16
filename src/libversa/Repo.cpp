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

    return Result<bool>::from(
        std::filesystem::create_directory("./.versa"),
        "Failed to create directory '.versa'."
    );
}