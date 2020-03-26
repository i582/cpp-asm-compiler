cmake_minimum_required(VERSION 3.15)
project(cpp_asm)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_asm main.cpp src/lexer/token/token.h src/lexer/lexer.h src/error-handle/error-handle.h src/error-handle/error-handle.cpp)