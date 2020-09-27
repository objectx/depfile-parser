/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */
#include "utility.hpp"
#include <depfile-parser.hpp>

#include <cassert>
#include <string>
#include <string_view>
#include <tuple>

using namespace DependencyFileParser::detail;

namespace {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-array-to-pointer-decay"
    tokenizer_result_t fetch_token (std::string_view src) {
        assert (! src.empty ());
        assert (! isspace (src[0]));

        std::string tok;
        while (! src.empty ()) {
            if (isspace (src[0]) != 0) {
                return {tok, skip_comment_and_space (src)};
            }
            if (src[0] == '\\') {
                src = src.substr (1);
                if (src.empty ()) {
                    return {tok, src};
                }
                tok.push_back (src[0]);
                src = src.substr (1);
                continue;
            }
            tok.push_back (src[0]);
            src = src.substr (1);
        }
        assert (src.empty ());
        return {std::move (tok), src};
    }
#pragma clang diagnostic pop
}  // namespace

namespace DependencyFileParser {
    Result Parse (std::string_view src) { return parse (fetch_token, src); }
}  // namespace DependencyFileParser
