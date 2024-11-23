cmake_minimum_required(VERSION 3.28)

project(static-reflect)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(pp_test)
target_sources(pp_test PRIVATE test/pp_test.cpp)
target_link_libraries(pp_test PRIVATE gtest gtest_main)
