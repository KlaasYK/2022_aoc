# Copyright 2022 Klaas Kliffen
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required (VERSION 3.16)

project(5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR src)

set(SOURCES1
    ${SOURCE_DIR}/main.1.cpp
)

set(SOURCES2
    ${SOURCE_DIR}/main.2.cpp
)

add_executable(aoc_${PROJECT_NAME}.1 ${SOURCES1})
add_executable(aoc_${PROJECT_NAME}.2 ${SOURCES2})
