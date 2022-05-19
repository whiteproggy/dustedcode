cmake_minimum_required(VERSION 3.5)

project(dustedcxx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    "."
    "../include"
)

set(test_files
    generators.h
    times.h
)

add_executable(dustedcxx main.cpp ${test_files})
