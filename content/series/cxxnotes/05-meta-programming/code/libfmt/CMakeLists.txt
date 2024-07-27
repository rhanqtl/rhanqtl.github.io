cmake_minimum_required(VERSION 3.25)

project(libfmt CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(libfmt_test)
target_sources(libfmt_test PRIVATE
    libfmt_test.cpp
)
target_link_libraries(libfmt_test PRIVATE
    gtest gtest_main
)
