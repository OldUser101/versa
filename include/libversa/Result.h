// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#define VERSA_TRY(expr) ({ auto _res = (expr); if (!_res.ok()) return res; _res.value; })

namespace libversa {

template<typename T>
struct Result {
    T value;
    std::string msg;
    bool ok() const { return msg.empty(); }

    static Result<T> success(T val) {
        return { std::move(val), "" };
    }

    static Result<T> failure(std::string msg) {
        return { {}, std::move(msg) };
    }

    static Result<T> from(T val, std::string msg) {
        return success(std::move(val));
    }
};

template<>
inline Result<bool> Result<bool>::from(bool val, std::string msg) {
    if (val) {
        return success(true);
    } else {
        return failure(std::move(msg));
    }
};

template<>
inline Result<int> Result<int>::from(int val, std::string msg) {
    if (val == 0) {
        return success(0);
    } else {
        return failure(std::move(msg));
    }
};

};