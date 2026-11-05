# tdu - ReXGlue Recompiled Project
#
# SDK boilerplate lives in generated/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(tdu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)

# Sources
set(TDU_SOURCES
    src/main.cpp
)

if(WIN32)
    add_executable(tdu WIN32 ${TDU_SOURCES})
else()
    add_executable(tdu ${TDU_SOURCES})
endif()

rexglue_setup_target(tdu)
