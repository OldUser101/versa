cmake_minimum_required(VERSION 3.15)
project(versa LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB_RECURSE LIBVERSA_SOURCES src/libversa/*.cpp)

add_library(libversa STATIC ${LIBVERSA_SOURCES})

file(GLOB_RECURSE CLI_SOURCES src/versa/*.cpp)
add_executable(versa ${CLI_SOURCES})

target_link_libraries(versa PRIVATE libversa)