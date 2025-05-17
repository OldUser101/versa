// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iomanip>

#include <libversa/Repo.h>

#include <versa/ConsoleLogger.h>

namespace versa {

class Cli {
private:
    ConsoleLogger logger = ConsoleLogger(false);
    libversa::Repo repo = libversa::Repo(this->logger);
    int exitCode = 0;
public:
    Cli(int argc, char* argv[]);
    int get_exit_code();
};

};