/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */
#include <catch2/catch.hpp>

#include <depfile-parser.hpp>

using namespace DependencyFileParser;
TEST_CASE ("make style dependency", "[make]") {
    SECTION ("empty input") {
        const std::string input;
        auto const &result = Parse (input);
        REQUIRE_FALSE (result);
    }
    SECTION ("empty but contains comments") {
        const std::string input { R"===(
# Copyright (c) 2020 Masashi Fujita All rights reserved.

#
# mokekek
)==="};
        auto const &result = Parse (input);
        REQUIRE_FALSE (result);
    }
    SECTION ("malformed dependency") {
        const std::string input { R"===(
# mokeke

        foo)==="};
        auto const &result = Parse (input);
        REQUIRE_FALSE (result);
    }

    SECTION ("simple dependency") {
        const std::string input { "abc : def"};
        auto const &result = Parse (input);

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }
    SECTION ("simple with continuation") {
        const std::string input { "abc :\\\r\n def"};
        auto const &result = Parse (input);

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }

    SECTION ("simple with continuation 2") {
        const std::string input { "abc :\\\r\n def \\\r\n"};
        auto const &result = Parse (input);

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 1);
        REQUIRE (prereq [0] == "def");
    }
    SECTION ("simple with continuation and stray '\\'") {
        const std::string input { "abc :\\\r\n def \\\r"};
        auto const &result = Parse (input);

        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "abc");
        REQUIRE (prereq.size () == 2);
        REQUIRE (prereq [0] == "def");
        REQUIRE (prereq [1] == "\r");   // Is it really needed?
    }
    SECTION ("large input") {
        const std::string input { R"===(
# Test dep.

z/sample.h: \
  /Users/objectx/Workspace/GitLab/GT/application/tools/ref_parser/standalone/sample.h \
  Dummy\ File.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/memory \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__config \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/type_traits \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cstddef \
  /usr/local/Cellar/llvm/7.0.1/lib/clang/7.0.1/include/stddef.h \
  /usr/local/Cellar/llvm/7.0.1/lib/clang/7.0.1/include/__stddef_max_align_t.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__nullptr \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/typeinfo \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/exception \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cstdlib \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/stdlib.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdlib.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/Availability.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/AvailabilityInternal.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_symbol_aliasing.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_posix_availability.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/_types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/_types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_pthread/_pthread_types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/wait.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_pid_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_id_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/signal.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/appleapiopts.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/signal.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/signal.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/_mcontext.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/_mcontext.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/mach/machine/_structs.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/mach/i386/_structs.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/types.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_int8_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_int16_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_int32_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_int64_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_u_int8_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_u_int16_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_u_int32_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_u_int64_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_intptr_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_uintptr_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_pthread/_pthread_attr_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_sigaltstack.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_ucontext.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_sigset_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_size_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_uid_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/resource.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/stdint.h \
  /usr/local/Cellar/llvm/7.0.1/lib/clang/7.0.1/include/stdint.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdint.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_uint8_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_uint16_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_uint32_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_uint64_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_intmax_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_uintmax_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_timeval.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/endian.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/endian.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_endian.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/libkern/_OSByteOrder.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/libkern/i386/_OSByteOrder.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/alloca.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_ct_rune_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_rune_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_wchar_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_null.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/malloc/_malloc.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_dev_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_mode_t.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cstdint \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/new \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/utility \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__tuple \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/initializer_list \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cstring \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/string.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/string.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_ssize_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/strings.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__debug \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/limits \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__undef_macros \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/iterator \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/iosfwd \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/wchar.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/wchar.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_mbstate_t.h \
  /usr/local/Cellar/llvm/7.0.1/lib/clang/7.0.1/include/stdarg.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/stdio.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdio.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_stdio.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_va_list.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/stdio.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_off_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/time.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_clock_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_time_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_timespec.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_wctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/__wctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_wint_t.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_wctype_t.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/ctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/ctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_ctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/runetype.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__functional_base \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/tuple \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/stdexcept \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cassert \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/assert.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/atomic \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/string \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/string_view \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__string \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/algorithm \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/functional \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cstdio \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cwchar \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cwctype \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/cctype \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/wctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/wctype.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/_types/_wctrans_t.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/vector \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__bit_reference \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/climits \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/limits.h \
  /usr/local/Cellar/llvm/7.0.1/lib/clang/7.0.1/include/limits.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/limits.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/machine/limits.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/limits.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/i386/_limits.h \
  /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/syslimits.h \
  /usr/local/Cellar/llvm/7.0.1/include/c++/v1/__split_buffer
)==="};
        auto const &result = Parse (input);
        REQUIRE (result);
        auto const &    prereq = result.prerequisites ();
        REQUIRE (result.target () == "z/sample.h");
        REQUIRE (prereq.size () == 143);
        REQUIRE (prereq [0] == "/Users/objectx/Workspace/GitLab/GT/application/tools/ref_parser/standalone/sample.h");
        REQUIRE (prereq [1] == "Dummy File.h");
        REQUIRE (prereq [142] == "/usr/local/Cellar/llvm/7.0.1/include/c++/v1/__split_buffer");
    }
}
