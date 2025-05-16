// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <string>

namespace libversa {

enum ErrorLevel {
    NONE,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    virtual ~Logger() = default;

    virtual void log(std::string msg, ErrorLevel l) = 0;
};

};