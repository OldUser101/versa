// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <filesystem>

#include <libversa/Result.h>

namespace libversa {

using Blake3Hash = std::vector<uint8_t>;

enum ObjectType {
    BLOB,
    TREE,
    COMMIT,
    VIEW
};

class Object {
private:
    ObjectType type;
    std::vector<uint8_t> data;
public:
    Object(ObjectType t, std::vector<uint8_t> data): type(t), data(data) {}

    ObjectType get_type();
    Blake3Hash hash_blake3();

    std::vector<uint8_t> serialize();
};

class ObjectContent {
protected:
    ObjectType type;
public:
    explicit ObjectContent(ObjectType t): type(t) {};

    virtual Object to_object() const = 0;
};

class Blob: public ObjectContent {
private:
    std::vector<uint8_t> data;
public:
    Blob(const std::vector<uint8_t>& d) : ObjectContent(ObjectType::BLOB), data(d) {}

    Object to_object() const override;
};

};