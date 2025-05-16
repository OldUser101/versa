// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <iostream>
#include <vector>
#include <string>

#include <versa/Cli.h>

using namespace versa;

Cli::Cli(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " <subcommand>" << std::endl;
        this->exitCode = 1;
        return;
    }

    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    if (args[1] == "init") {
        libversa::Result<bool> r = this->repo.init();
        if (!r.ok()) {
            this->logger.log(r.msg, libversa::ERROR);
            this->exitCode = 1;
            return;
        }
    } else {
        this->logger.log("Unknown subcommand '" + args[1] + "'", libversa::ERROR);
        this->exitCode = 1;
        return;        
    }

    this->exitCode = 0;
}

int Cli::get_exit_code() {
    return this->exitCode;
}