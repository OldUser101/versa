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
        }
        return;
    }

    Result<bool> rOpen = this->repo.open_repo("./");
    if (!rOpen.ok()) {
        this->logger.log("The current directory does not contain a VERSA repository.", ERROR);
        this->exitCode = 1;
        return;         
    }
    
    if (args[1] == "hash-object") {
        Result<std::vector<uint8_t>> r = Util::read_file(argv[2]);
        if (!r.ok()) {
            this->logger.log(r.msg, ERROR);
            this->exitCode = 1;
            return;          
        }

        Blob bObj = Blob(r.get());
        Object obj = bObj.to_object();
        
        Result<bool> r2 = this->repo.hash_object(obj, true);
        if (!r2.ok()) {
            this->logger.log(r2.msg, ERROR);
            this->exitCode = 1;      
        }

        return;
    } else if (args[1] == "cat-file") {
        Result<std::unique_ptr<ObjectContent>> rContent = this->repo.cat_file(libversa::BLOB, args[2]);
        if (!rContent.ok()) {
            this->logger.log(rContent.msg, ERROR);
            this->exitCode = 1;
            return;          
        }

        std::unique_ptr<ObjectContent> content = std::move(rContent.get());
        content->print();

        return;
    }

    this->logger.log("Unknown subcommand '" + args[1] + "'", ERROR);
    this->exitCode = 1;
}

int Cli::get_exit_code() {
    return this->exitCode;
}