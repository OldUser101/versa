// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace libversa {

using Blake3Hash = std::vector<uint8_t>;

enum ObjectType {
    BLOB,
    TREE,
    COMMIT,
    TAG
};

class Object {
protected:
    ObjectType type;
public:
    explicit Object(ObjectType t): type(t) {}
    virtual ~Object() = default;

    ObjectType get_type() const;
    Blake3Hash hash_blake3() const;

    virtual std::vector<uint8_t> serialize() const = 0;
};

class Blob: public Object {
private:
    std::vector<uint8_t> data;
public:
    explicit Blob(const std::vector<uint8_t>& d) : Object(ObjectType::BLOB), data(d) {}

    std::vector<uint8_t> serialize() const override;
};

};