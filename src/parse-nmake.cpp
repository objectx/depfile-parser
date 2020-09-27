/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */

#include "depfile-parser.hpp"
#include "utility.hpp"

#include <cassert>
#include <cctype>
#include <tuple>

using namespace DependencyFileParser::detail;

namespace {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-array-to-pointer-decay"
    tokenizer_result_t fetch_token (std::string_view src) {
        assert (! src.empty ());
        assert (! isspace (src[0]));

        if (src[0] == '"') {
            size_t pos = 1;
            while (pos < src.size ()) {
                if (src[pos] == '"') {
                    return {std::string (src.substr (1, pos - 1)), skip_comment_and_space (src.substr (pos + 1))};
                }
                ++pos;
            }
            assert (pos == src.size ());
            return {std::string {src.substr (1)}, src.substr (pos)};
        }
        size_t pos = 1;
        while (pos < src.size ()) {
            if (isspace (src[pos]) != 0) {
                return {std::string {src.substr (0, pos)}, skip_comment_and_space (src.substr (pos + 1))};
            }
            ++pos;
        }
        assert (pos == src.size ());
        return {std::string {src}, src.substr (pos)};
    }
#pragma clang diagnostic pop
}  // namespace

namespace DependencyFileParser {
    Result ParseNMakeStyle (std::string_view src) { return parse (fetch_token, src); }
}  // namespace DependencyFileParser
