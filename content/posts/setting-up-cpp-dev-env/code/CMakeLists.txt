cmake_minimum_required(VERSION 3.25)

project(vscode-clangd-demo CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIB_NAME demo_support)
add_library(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE src/Support/Strings.cpp)

set(EXE_NAME demo)
add_executable(${EXE_NAME})
target_sources(${EXE_NAME} PRIVATE src/main.cpp)
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
