/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */
#include <doctest/doctest.h>

#include <depfile-parser.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"

using namespace DependencyFileParser;
TEST_CASE ("nmake style dependency") {
    SUBCASE ("empty input") {
        auto const &result = ParseNMakeStyle ("");
        REQUIRE_FALSE (result);
    }
    SUBCASE ("empty but contains comments") {
        auto const &result = ParseNMakeStyle (R"===(
# Copyright (c) 2019 Masashi Fujita. All rights reserved.

#
# mokekek
)===");
        REQUIRE_FALSE (result);
    }
    SUBCASE ("malformed dependency") {
        auto const &result = ParseNMakeStyle (R"===(
# mokeke

        foo)===");
        REQUIRE_FALSE (result);
    }

    SUBCASE ("simple dependency") {
        auto const &result = ParseNMakeStyle ("abc : def");

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }
    SUBCASE ("simple with continuation") {
        auto const &result = ParseNMakeStyle ("abc :\\\r\n def");

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }

    SUBCASE ("simple with continuation 2") {
        auto const &result = ParseNMakeStyle ("abc :\\\r\n def \\\r\n");

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }
    SUBCASE ("simple with continuation and stray '\\'") {
        auto const &result = ParseNMakeStyle ("abc :\\\r\n def \\\r");

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 2);
        REQUIRE (prereq [0] == "def");
        REQUIRE (prereq [1] == "\\");   // Is really needed?
    }
    SUBCASE ("large input") {
        auto const &result = ParseNMakeStyle (R"###(
"z/sample.h": c:\dev\ref_tbl_gen\sample.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\memory \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xmemory \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xmemory0 \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cstdint \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\yvals.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\yvals_core.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xkeycheck.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\crtdefs.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\sal.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\concurrencysal.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vadefs.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\crtdbg.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime_new_debug.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime_new.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\use_ansi.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\stdint.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cstdlib \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\stdlib.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_malloc.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_search.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\stddef.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wstdlib.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\limits.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\math.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_math.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\limits \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\ymath.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cfloat \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\float.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\climits \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cmath \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xtgmath.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xtr1common \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cwchar \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\wchar.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_memcpy_s.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\errno.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime_string.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wconio.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_stdio_config.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wctype.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wdirect.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wio.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_share.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wprocess.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wstdio.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wstring.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_wtime.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\sys/stat.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\sys/types.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xstddef \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cstddef \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\initializer_list \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\new \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\exception \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\type_traits \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\malloc.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime_exception.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\eh.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_terminate.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xutility \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\utility \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\iosfwd \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cstdio \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\stdio.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cstring \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\string.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\corecrt_memory.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xatomic0.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\intrin0.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\typeinfo \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vcruntime_typeinfo.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\string \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\istream \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\ostream \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\ios \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xlocnum \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\streambuf \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xiosbase \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xlocale \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\stdexcept \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xstring \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xlocinfo \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xlocinfo.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\ctype.h" \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\locale.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xfacet \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\system_error \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\cerrno \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xcall_once.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xerrc.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\share.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xstring_insert.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\vector \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\algorithm \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\assert.h" \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\iterator \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\deque \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\list \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\set \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xtree \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\map \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\tuple \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\fstream \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\iostream
)###");
        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "z/sample.h");
        REQUIRE (prereq.size () == 108);
        REQUIRE (prereq[0] == "c:\\dev\\ref_tbl_gen\\sample.h");
        REQUIRE (prereq [13] == "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.17763.0\\ucrt\\corecrt.h");
        REQUIRE (prereq [100] == "c:\\tools\\MSVS\\2017\\Professional\\VC\\Tools\\MSVC\\14.16.27023\\include\\deque");
    }
}

#pragma clang diagnostic pop
