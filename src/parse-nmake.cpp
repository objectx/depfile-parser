/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */

#include "depfile-parser.hpp"
#include "utility.hpp"

#include <cctype>
#include <cassert>
#include <tuple>

using namespace DependencyFileParser::detail;

namespace {
    const char *    skip_comment_and_space (const char *p, const char *end_p) {
        while (p < end_p) {
            auto q = skip_space (p, end_p);
            if (q == nullptr) {
                return nullptr ;
            }
            if (*q != '#') {
                return q;
            }
            // *q == '#'.  Skip to EOL.
            p = skip_to_eol (q + 1, end_p);
            if (p == nullptr) {
                return nullptr;
            }
        }
        return nullptr;
    }

    std::tuple<std::string, const char *> fetch_token (const char *p, const char *end_p) {
        assert (p < end_p);
        assert (! isspace (*p));
        auto q = p;
        if (*q == '"') {
            auto s = ++q;
            while (q < end_p) {
                if (*q == '"') {
                    std::string token { s, static_cast<size_t> (q - s) };
                    q = skip_comment_and_space (q + 1, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);
            return {std::string {s, static_cast<size_t> (q - s)}, nullptr};
        }
        else {
            while (q < end_p) {
                if (isspace (*q)) {
                    std::string token { p, static_cast<size_t> (q - p) };
                    q = skip_comment_and_space (q, end_p);
                    return {token, q};
                }
                ++q;
            }
            assert (q == end_p);
            return {std::string {p, static_cast<size_t> (q - p)}, nullptr};
        }
    }
}

namespace DependencyFileParser {
    std::optional<Result> ParseNMakeStyle (const char *p, size_t size) {
        auto const end_p = p + size;
        p = skip_comment_and_space (p, end_p);
        if (p == nullptr) {
            // Empty definitions.
            return {};
        }
        // 1st: Obtains target.
        auto const &target = fetch_token (p, end_p);
        p = std::get<1> (target);
        if (p == nullptr) {
            // Malformed dependency definition.
            return {};
        }
        if (*p != ':') {
            return {};  // Malformed.
        }
        p = skip_space (p + 1, end_p);
        if (p == nullptr) {
            return {};
        }
        // 2nd: Obtains dependents.
        std::vector<std::string>    deps;
        while (p < end_p) {
            auto dep = fetch_token (p, end_p);
            deps.emplace_back (std::get<0> (dep));
            p = std::get<1> (dep);
            if (p == nullptr) {
                break;
            }
        }
        return std::make_optional<Result> (std::get<0> (target), std::move (deps));
    }
}
