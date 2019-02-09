#include <catch2/catch.hpp>

#include <depfile-parser.hpp>

using namespace DependencyFileParser;
TEST_CASE ("nmake style dependency", "[nmake]") {
    SECTION ("empty input") {
        const std::string input;
        auto const &result = ParseNMakeStyle (input);
        REQUIRE_FALSE (result);
    }
    SECTION ("empty but contains comments") {
        const std::string input { R"===(
# Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.

#
# mokekek
)==="};
        auto const &result = ParseNMakeStyle (input);
        REQUIRE_FALSE (result);
    }
    SECTION ("malformed dependency") {
        const std::string input { R"===(
# mokeke

        foo)==="};
        auto const &result = ParseNMakeStyle (input);
        REQUIRE_FALSE (result);
    }

    SECTION ("simple dependency") {
        const std::string input { "abc : def"};
        auto const &result = ParseNMakeStyle (input);

        REQUIRE (result);
        REQUIRE (result->target () == "abc");
        REQUIRE (result->size () == 1);
        REQUIRE ((*result) [0] == "def");
    }
    SECTION ("simple with continuation") {
        const std::string input { "abc :\\\r\n def"};
        auto const &result = ParseNMakeStyle (input);

        REQUIRE (result);
        REQUIRE (result->target () == "abc");
        REQUIRE (result->size () == 1);
        REQUIRE ((*result) [0] == "def");
    }

    SECTION ("simple with continuation 2") {
        const std::string input { "abc :\\\r\n def \\\r\n"};
        auto const &result = ParseNMakeStyle (input);

        REQUIRE (result);
        REQUIRE (result->target () == "abc");
        REQUIRE (result->size () == 1);
        REQUIRE ((*result) [0] == "def");
    }
    SECTION ("simple with continuation and stray '\\'") {
        const std::string input { "abc :\\\r\n def \\\r"};
        auto const &result = ParseNMakeStyle (input);

        REQUIRE (result);
        REQUIRE (result->target () == "abc");
        REQUIRE (result->size () == 2);
        REQUIRE ((*result) [0] == "def");
        REQUIRE ((*result) [1] == "\\");
    }
    SECTION ("large input") {
        const std::string input { R"###(
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
  C:/dev/GT/pdistd/include\pdistd/pmf/byteblock.h \
  C:/dev/GT/pdistd/include\pdistd/pseudoreflection.h \
  C:/dev/GT/pdistd/include\pdistd/any.h \
  C:/dev/GT/pdistd/include\pdistd/compiler.h \
  C:/dev/GT/pdistd/include\pdistd/typeinfo.h \
  C:/dev/GT/pdistd/include\pdistd/is_base_and_derived.h \
  C:/dev/GT/pdistd/include\pdistd/type_traits_util.h \
  C:/dev/GT/pdistd/include\pdistd/meta/bool.h \
  C:/dev/GT/pdistd/include\pdistd/is_class.h \
  C:/dev/GT/pdistd/include\pdistd/type_traits.h \
  C:/dev/GT/pdistd/include\pdistd/types.h \
  C:/dev/GT/pdistd/include\pdistd/win32/types.h \
  C:/dev/GT/pdistd/include\pdistd/win32/../stddef.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/cat.h \
  C:/dev/GT/pdistd/include\pdistd/win32/../cdefs.h \
  C:/dev/GT/pdistd/include\pdistd/stdint.h \
  C:/dev/GT/pdistd/include\pdistd/meta/or.h \
  C:/dev/GT/pdistd/include\pdistd/meta/if.h \
  C:/dev/GT/pdistd/include\pdistd/meta/and.h \
  C:/dev/GT/pdistd/include\pdistd/is_pod.h \
  C:/dev/GT/pdistd/include\pdistd/is_empty.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\algorithm \
  C:/dev/GT/pdistd/include\pdistd/assert.h \
  C:/dev/GT/pdistd/include\pdistd/win32/assert.h \
  "C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\assert.h" \
  C:/dev/GT/pdistd/include\pdistd/win32/../psx/assert.h \
  C:/dev/GT/pdistd/include\pdistd/win32/../psx/../cdefs.h \
  C:/dev/GT/pdistd/include\pdistd/win32/../psx/../pdistd_exports.h \
  C:/dev/GT/pdistd/include\pdistd/extern.h \
  C:/dev/GT/pdistd/include\pdistd/enable_if.h \
  C:/dev/GT/pdistd/include\pdistd/forward_list.h \
  C:/dev/GT/pdistd/include\pdistd/namespace.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\iterator \
  C:/dev/GT/pdistd/include\pdistd/meta/type.h \
  C:/dev/GT/pdistd/include\pdistd/forward_list_node_traits.h \
  C:/dev/GT/pdistd/include\pdistd/message.h \
  C:/dev/GT/pdistd/include\pdistd/stdarg.h \
  C:/tools/SCE/ORBIS-SDK/6.000/host_tools/lib/clang\include\stdarg.h \
  C:/dev/GT/pdistd/include\pdistd/message/private.h \
  C:/dev/GT/pdistd/include\pdistd/message/../compiler.h \
  C:/dev/GT/pdistd/include\pdistd/message/../sleep.h \
  C:/dev/GT/pdistd/include\pdistd/pdistd_exports.h \
  C:/dev/GT/pdistd/include\pdistd/message/inline.h \
  C:/dev/GT/pdistd/include\pdistd/forward_list_node.h \
  C:/dev/GT/pdistd/include\pdistd/in_place_factory.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/iteration.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/iteration_iterate.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/dec.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/inc.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/tuple_elem.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/array_elem.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/array_data.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/array_size.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/detail/slot_def.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/iteration_self.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/repeat.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/enum.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/comma_if.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/comma.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/empty.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/if.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/bool.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/enum_params.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/enum_binary_params.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/expand.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/enum_trailing_params.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/enum_trailing_binary_params.h \
  C:/dev/GT/pdistd/include\pdistd/preprocessor/detail/iter_forward1.h \
  C:/dev/GT/pdistd/include\pdistd/instance_buffer.h \
  C:/dev/GT/pdistd/include\pdistd/aligned.h \
  C:/dev/GT/pdistd/include\pdistd/alignof.h \
  C:/dev/GT/pdistd/include\pdistd/cdefs.h \
  C:/dev/GT/pdistd/include\pdistd/alignedi.h \
  C:/dev/GT/pdistd/include\pdistd/static_assert.h \
  C:/dev/GT/pdistd/include\pdistd/number.h \
  C:/dev/GT/pdistd/include\pdistd/stl_container.h \
  C:/dev/GT/pdistd/include\pdistd/stl_allocator.h \
  C:/dev/GT/pdistd/include\pdistd/stddef.h \
  C:/dev/GT/pdistd/include\pdistd/stdlib.h \
  C:/dev/GT/pdistd/include\pdistd/win32/stdlib.h \
  C:/dev/GT/pdistd/include\pdistd/win32/../compiler.h \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\deque \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\list \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\set \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xtree \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\map \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\tuple \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\fstream \
  c:\tools\MSVS\2017\Professional\VC\Tools\MSVC\14.16.27023\include\iostream
)###"};
        auto const &result = ParseNMakeStyle (input);
        REQUIRE (result);
        REQUIRE (result->target () == "z/sample.h");
        REQUIRE (result->size () == 186);
        REQUIRE ((*result) [13] == "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.17763.0\\ucrt\\corecrt.h");
        REQUIRE ((*result) [185] == "c:\\tools\\MSVS\\2017\\Professional\\VC\\Tools\\MSVC\\14.16.27023\\include\\iostream");
    }
}
