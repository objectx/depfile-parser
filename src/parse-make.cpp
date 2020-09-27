/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */
#include "utility.hpp"
#include <depfile-parser.hpp>

#include <cassert>
#include <string>
#include <tuple>

using namespace DependencyFileParser::detail;

namespace {
    std::tuple<std::string, const char *> fetch_token (const char *p, const char *end_p) {
        assert (p < end_p);       // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
        assert (! isspace (*p));  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)

        std::string tok;
        while (p < end_p) {
            if (isspace (*p) != 0) {
                return {std::move (tok), skip_comment_and_space (p, end_p)};
            }
            if (*p == '\\') {
                ++p;
                if (end_p <= p) {
                    tok.push_back ('\\');
                    return {std::move (tok), end_p};
                }
                tok.push_back (*p++);
                continue;
            }
            tok.push_back (*p++);
        }
        assert (p == end_p);  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
        return {std::move (tok), end_p};
    }
}  // namespace

namespace DependencyFileParser {

    Result Parse (const char *src, size_t size) { return parse (fetch_token, src, src + size); }
}  // namespace DependencyFileParser
