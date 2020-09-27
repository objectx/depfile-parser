
cmake_minimum_required (VERSION 3.16)

project (depfile-parser LANGUAGES CXX)

option (DEPFILE_PARSER_ENABLE_TESTS "Enable test suites" YES)
option (DEPFILE_PARSER_ENABLE_SANITIZERS "Enable sanitizers" YES)
option (DEPFILE_PARSER_ENABLE_INSTRUMENTATION "Enable coverage/profiling" YES)

function (setvar_default var_)
    if (NOT DEFINED ${var_})
        set (${var_} ${ARGN} PARENT_SCOPE)
    endif ()
endfunction ()

setvar_default (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
setvar_default (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
setvar_default (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (DEPFILE_PARSER_ENABLE_TESTS)
    include (${CMAKE_BINARY_DIR}/conan_paths.cmake)
    enable_testing ()
endif ()

if (DEPFILE_PARSER_ENABLE_SANITIZERS)
    set (t_ depfile-parser-sanitizers)
    add_library (${t_} INTERFACE)
    target_compile_options (${t_} INTERFACE
                            -fsanitize=address$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:,undefined>)
    target_link_options (${t_} INTERFACE
                         -fsanitize=address$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:,undefined>)
endif ()
if (DEPFILE_PARSER_ENABLE_INSTRUMENTATION)
    set (t_ depfile-parser-instrumentation)
    add_library (${t_} INTERFACE)
    target_compile_options (${t_} INTERFACE $<$<CXX_COMPILER_ID:Clang,AppleClang>:-fprofile-instr-generate;-fcoverage-mapping>)
    target_link_options (${t_} INTERFACE $<$<CXX_COMPILER_ID:Clang,AppleClang>:-fprofile-instr-generate;-fcoverage-mapping>)
endif ()

list (INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set (dep_parser_ depfile-parser)
add_library (${dep_parser_} STATIC
             include/depfile-parser.hpp
             src/utility.cpp
             src/utility.hpp
             src/parse-make.cpp
             src/parse-nmake.cpp)
target_include_directories (${dep_parser_} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features (${dep_parser_} PUBLIC cxx_std_17)
if (TARGET depfile-parser-sanitizers)
    target_link_libraries (${dep_parser_} PRIVATE depfile-parser-sanitizers)
endif ()
if (TARGET depfile-parser-instrumentation)
    target_link_libraries(${dep_parser_} PRIVATE depfile-parser-instrumentation)
endif ()

if (DEPFILE_PARSER_ENABLE_TESTS)
    find_package (doctest)

    add_subdirectory (test)
endif ()
