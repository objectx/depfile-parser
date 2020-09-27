
cmake_minimum_required (VERSION 3.16)

project (depfile-parser LANGUAGES CXX)

function (setvar_default var_)
    if (NOT DEFINED ${var_})
        set (${var_} ${ARGN} PARENT_SCOPE)
    endif ()
endfunction ()

setvar_default (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
setvar_default (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
setvar_default (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

list (INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

enable_testing ()

set (CMAKE_FOLDER _extras)

set (RC_ENABLE_CATCH YES)
# add_subdirectory (ext/rapidcheck)

include_directories (ext)

set (CMAKE_FOLDER Parser)
set (dep_parser_ depfile-parser)
add_library (${dep_parser_} STATIC include/depfile-parser.hpp
                                   src/utility.cpp
                                   src/utility.hpp
                                   src/parse-make.cpp
                                   src/parse-nmake.cpp)
target_include_directories (${dep_parser_} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features    (${dep_parser_} PUBLIC cxx_std_14)

add_subdirectory (test)
