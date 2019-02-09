
cmake_minimum_required (VERSION 3.13)

set (CMAKE_FOLDER "Parser Tests")
set (test_dep_parser_ test-depfile-parser)

add_executable (${test_dep_parser_} main.cpp nmake.cpp make.cpp)
target_link_libraries (${test_dep_parser_} PRIVATE depfile-parser)
target_compile_features (${test_dep_parser_} PRIVATE cxx_std_14)

add_test (NAME parser-test
          COMMAND ${test_dep_parser_} -r compact)
