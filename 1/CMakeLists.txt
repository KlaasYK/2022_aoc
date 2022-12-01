# Copyright 2022 Klaas Kliffen
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required (VERSION 3.16)

project(1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
)

add_executable(aoc_${PROJECT_NAME} ${SOURCES})
