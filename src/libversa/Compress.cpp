// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Compress.h>
#include <iostream>

using namespace libversa;

Result<std::vector<uint8_t>> Compress::compress_bytes(std::vector<uint8_t> bytes) {
    size_t fSize = bytes.size();
    const size_t cBufSize = ZSTD_compressBound(fSize);
    std::vector<uint8_t> cBuf(cBufSize);

    const size_t cSize = ZSTD_compress(cBuf.data(), cBufSize, bytes.data(), fSize, 1);
    unsigned int err = ZSTD_isError(cSize);

    if (err) {
        return Result<std::vector<uint8_t>>::failure(std::string("ZSTD: ") + ZSTD_getErrorString(ZSTD_getErrorCode(cSize)));
    }

    cBuf.resize(cSize);

    return Result<std::vector<uint8_t>>::success(cBuf);
}

Result<std::vector<uint8_t>> Compress::decompress_bytes(std::vector<uint8_t> bytes) {
    unsigned long long const rSize = ZSTD_getFrameContentSize(bytes.data(), bytes.size());
    if (rSize == ZSTD_CONTENTSIZE_ERROR || rSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        return Result<std::vector<uint8_t>>::failure("Invalid or corrupt object data.");
    }

    std::vector<uint8_t> rData(rSize);

    size_t const dSize = ZSTD_decompress(rData.data(), rData.size(), bytes.data(), bytes.size());

    unsigned int err = ZSTD_isError(dSize);
    if (err) {
        return Result<std::vector<uint8_t>>::failure(std::string("ZSTD: ") + ZSTD_getErrorString(ZSTD_getErrorCode(dSize)));
    }

    if (dSize != rSize) {
        return Result<std::vector<uint8_t>>::failure("Invalid or corrupt object data.");
    }

    return Result<std::vector<uint8_t>>::success(rData);
}