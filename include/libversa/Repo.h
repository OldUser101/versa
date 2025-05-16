// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <libversa/Result.h>
#include <libversa/Logger.h>

namespace libversa {

class Repo {
private:
    Logger& logger;
public:
    Repo(Logger& logger);
    Result<bool> init();
};

};