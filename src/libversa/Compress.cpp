// SPDX-License-Identifier: GPL-3.0-or-later
// VERSA – A fast, branchless version control system
// Copyright (C) 2025 Nathan Gill

#include <libversa/Compress.h>

using namespace libversa;

Result<std::vector<uint8_t>> Compress::compress_bytes(std::vector<uint8_t> bytes) {
    size_t fSize = bytes.size();
    const size_t cBufSize = ZSTD_compressBound(fSize);
    std::vector<uint8_t> cBuf(cBufSize);

    const size_t cSize = ZSTD_compress(cBuf.data(), cBufSize, bytes.data(), fSize, 1);
    unsigned int err = ZSTD_isError(cSize);

    if (err) {
        return Result<std::vector<uint8_t>>::failure(ZSTD_getErrorString(ZSTD_getErrorCode(cSize)));
    }

    return Result<std::vector<uint8_t>>::success(cBuf);
}