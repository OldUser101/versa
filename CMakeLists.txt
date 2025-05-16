cmake_minimum_required(VERSION 3.15)
project(versa LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(extern)

file(GLOB_RECURSE LIBVERSA_SOURCES src/libversa/*.cpp)

add_library(libversa STATIC ${LIBVERSA_SOURCES})
set_target_properties(libversa PROPERTIES PREFIX "")

file(GLOB_RECURSE CLI_SOURCES src/versa/*.cpp)

set(BLAKE3_SOURCES 
    extern/BLAKE3/c/blake3.c
    extern/BLAKE3/c/blake3_dispatch.c
    extern/BLAKE3/c/blake3_portable.c
)

add_executable(versa ${CLI_SOURCES} ${BLAKE3_SOURCES})

add_custom_target(
    zstd
    COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/extern/zstd
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/zstd/lib/libzstd.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extern/zstd
    COMMENT "Building zstd"
)

add_library(libzstd STATIC IMPORTED)
set_target_properties(libzstd PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/zstd/lib/libzstd.a
)

add_dependencies(libzstd zstd)

target_compile_definitions(versa PRIVATE
    BLAKE3_NO_SSE2
    BLAKE3_NO_SSE41
    BLAKE3_NO_AVX2
    BLAKE3_NO_AVX512
)

target_link_libraries(versa PRIVATE libversa libzstd)
target_include_directories(versa PRIVATE extern/zstd/lib)