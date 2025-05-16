// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <versa/Cli.h>

int main(int argc, char* argv[]) {
    return versa::Cli(argc, argv).get_exit_code();
}