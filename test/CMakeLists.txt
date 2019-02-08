
cmake_minimum_required (VERSION 3.13)

set (CMAKE_FOLDER "Parser Tests")
set (test_dep_parser_ test-depfile-parser)

add_executable (${test_dep_parser_} main.cpp nmake.cpp)
target_link_libraries (${test_dep_parser_} PRIVATE depfile-parser)

add_test (NAME parser-test
          COMMAND ${test_dep_parser_} -r compact)
