cmake_minimum_required(VERSION 3.20)

project(pspsdk_toolchain_smoke LANGUAGES C CXX ASM)

add_library(cxx_include_probe OBJECT cxx_include_probe.cpp)
add_library(asm_include_probe OBJECT asm_include_probe.S)

add_executable(default_link_probe default_link_probe.c)
