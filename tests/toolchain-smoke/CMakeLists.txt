cmake_minimum_required(VERSION 3.20)

project(pspsdk_toolchain_smoke LANGUAGES CXX ASM)

add_library(cxx_include_probe OBJECT cxx_include_probe.cpp)
add_library(asm_include_probe OBJECT asm_include_probe.S)
