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
    src/any360_graphics.cpp
)

if(WIN32)
    add_executable(tdu WIN32 ${TDU_SOURCES})
else()
    add_executable(tdu ${TDU_SOURCES})
endif()

rexglue_setup_target(tdu)

# Any360 (github.com/GaetanVigner/Any360): its layers built from its checkout with this toolchain; the GPU goes
# through them with --any360_gpu=true (src/any360_graphics.cpp).
set(TDU_ANY360_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Any360" CACHE PATH "Any360 checkout")
add_subdirectory("${TDU_ANY360_DIR}/layer/sys" any360 EXCLUDE_FROM_ALL)
target_link_libraries(tdu PRIVATE any360_host_rexglue)
