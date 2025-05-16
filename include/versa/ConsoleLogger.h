// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <iostream>

#include <libversa/Logger.h>

namespace versa {

class ConsoleLogger: public libversa::Logger {
private:
    bool suppressInfo;
public:
    ConsoleLogger();
    ConsoleLogger(bool suppressInfo);

    void log(std::string msg, libversa::ErrorLevel l) override;
};

};