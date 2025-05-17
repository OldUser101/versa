// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <versa/Cli.h>

#include <libversa/Result.h>
#include <libversa/Hash.h>
#include <libversa/Object.h>
#include <libversa/Util.h>

using namespace versa;
using namespace libversa;

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
        Result<bool> r = this->repo.init();
        if (!r.ok()) {
            this->logger.log(r.msg, ERROR);
            this->exitCode = 1;
            return;
        }
    } else if (args[1] == "hash-object") {
        Result<std::vector<uint8_t>> r = Util::read_file(argv[2]);
        if (!r.ok()) {
            this->logger.log(r.msg, ERROR);
            this->exitCode = 1;
            return;          
        }

        Blob obj = Blob(r.value);
        Blake3Hash hash = obj.hash_blake3();
        std::string hashStr = Util::hash_to_string(hash);
        std::cout << hashStr << std::endl;

        Result<bool> r2 = obj.hash_object();
        if (!r2.ok()) {
            this->logger.log(r2.msg, ERROR);
            this->exitCode = 1;
            return;
        }
    } else {
        this->logger.log("Unknown subcommand '" + args[1] + "'", ERROR);
        this->exitCode = 1;
        return;        
    }

    this->exitCode = 0;
}

int Cli::get_exit_code() {
    return this->exitCode;
}