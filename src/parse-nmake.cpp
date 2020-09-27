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
    std::tuple<std::string, const char *> fetch_token (const char *p, const char *end_p) {
        assert (p < end_p);       // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
        assert (! isspace (*p));  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
        auto const *q = p;
        if (*q == '"') {
            auto const *s = ++q;
            while (q < end_p) {
                if (*q == '"') {
                    std::string token {s, static_cast<size_t> (q - s)};
                    q = skip_comment_and_space (q + 1, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
            return {std::string {s, static_cast<size_t> (q - s)}, end_p};
        }
        else {
            while (q < end_p) {
                if (isspace (*q) != 0) {
                    std::string token {p, static_cast<size_t> (q - p)};
                    q = skip_comment_and_space (q, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
            return {std::string {p, static_cast<size_t> (q - p)}, end_p};
        }
    }
}  // namespace

namespace DependencyFileParser {
    Result ParseNMakeStyle (std::string_view src) { return parse (fetch_token, src.begin (), src.end ()); }
}  // namespace DependencyFileParser
