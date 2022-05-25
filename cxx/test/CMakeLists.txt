cmake_minimum_required(VERSION 3.5)

project(dustedcxx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_CXX_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../include")

include_directories(
    "."
    "${PROJECT_CXX_INCLUDE_DIR}"
)

set(include_files
    "${PROJECT_CXX_INCLUDE_DIR}/comment.h"
    "${PROJECT_CXX_INCLUDE_DIR}/sort.h"
    "${PROJECT_CXX_INCLUDE_DIR}/files.h"
    "${PROJECT_CXX_INCLUDE_DIR}/outsort.h"
    )

set(test_files
    generators.h
    times.h
    main.cpp
)

add_executable(dustedcxx ${test_files} ${include_files})
