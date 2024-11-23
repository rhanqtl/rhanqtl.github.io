cmake_minimum_required(VERSION 3.28)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(lexer)
target_sources(lexer PRIVATE
    src/Lexer.cpp
    src/Token.cpp
)

