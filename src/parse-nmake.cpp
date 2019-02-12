/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */

#include "depfile-parser.hpp"
#include "utility.hpp"

#include <cassert>
#include <cctype>
#include <tuple>

using namespace DependencyFileParser::detail;

namespace {
    std::tuple<std::string, const char*> fetch_token (const char* p, const char* end_p) {
        assert (p < end_p);
        assert (!isspace (*p));
        auto q = p;
        if (*q == '"') {
            auto s = ++q;
            while (q < end_p) {
                if (*q == '"') {
                    std::string token{s, static_cast<size_t> (q - s)};
                    q = skip_comment_and_space (q + 1, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);
            return {std::string{s, static_cast<size_t> (q - s)}, end_p};
        }
        else {
            while (q < end_p) {
                if (isspace (*q)) {
                    std::string token{p, static_cast<size_t> (q - p)};
                    q = skip_comment_and_space (q, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);
            return {std::string{p, static_cast<size_t> (q - p)}, end_p};
        }
    }
} // namespace

namespace DependencyFileParser {
    Result ParseNMakeStyle (const char* src, size_t size) {
        return parse (fetch_token, src, src + size);
    }
} // namespace DependencyFileParser
