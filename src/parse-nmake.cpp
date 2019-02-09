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
            return {std::string {s, static_cast<size_t> (q - s)}, end_p};
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
            return {std::string {p, static_cast<size_t> (q - p)}, end_p};
        }
    }
}

namespace DependencyFileParser {
    Result ParseNMakeStyle (const char* src, size_t size) {
        auto const end_p = src + size;
        src = skip_comment_and_space (src, end_p);
        if (src == end_p) {
            // Empty definitions.
            return {};
        }
        // 1st: Obtains target.
        auto const &target = fetch_token (src, end_p);
        src = std::get<1> (target);
        if (src == end_p) {
            // Malformed dependency definition.
            return {};
        }
        std::string target_path ;
        // Handle "<filepath>:" case.
        {
            auto const &p = std::get<0> (target);
            if (*std::rbegin (p) == ':') {
                target_path = p.substr (0, p.size () - 1);
            }
            else {
                target_path = std::get<0> (target);
                if (*src != ':') {
                    return {};  // Malformed.
                }
            }
        }
        src = skip_space (src + 1, end_p);
        if (src == end_p) {
            return {};
        }
        // 2nd: Obtains dependents.
        std::vector<std::string>    deps;
        while (src < end_p) {
            auto dep = fetch_token (src, end_p);
            deps.emplace_back (std::get<0> (dep));
            src = std::get<1> (dep);
            if (src == end_p) {
                break;
            }
        }
        return {std::move (target_path), std::move (deps)};
    }
} // namespace DependencyFileParser
