// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <versa/ConsoleLogger.h>

using namespace versa;

#define VERSA_COLOR_RESET std::string("\033[0m")
#define VERSA_COLOR_INFO std::string("\033[1;34m")
#define VERSA_COLOR_WARN std::string("\033[1;33m")
#define VERSA_COLOR_ERROR std::string("\033[1;31m")

ConsoleLogger::ConsoleLogger() {}

ConsoleLogger::ConsoleLogger(bool suppressInfo) {
    this->suppressInfo = suppressInfo;
}

void ConsoleLogger::log(std::string msg, libversa::ErrorLevel l) {
    if (l == libversa::INFO && this->suppressInfo) {
        return;
    }

    std::string errorType;

    switch(l) {
        case libversa::NONE:
            errorType = "";
            break;
        case libversa::INFO:
            errorType = "[" + VERSA_COLOR_INFO + "INFO" + VERSA_COLOR_RESET + "] ";
            break;
        case libversa::WARN:
            errorType = "[" + VERSA_COLOR_WARN + "WARN" + VERSA_COLOR_RESET + "] ";
            break;
        case libversa::ERROR:
            errorType = "[" + VERSA_COLOR_ERROR + "ERROR" + VERSA_COLOR_RESET + "] ";
            break;
        default:
            return;
    }

    std::cout << errorType << msg << std::endl;
}