/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */
#include "utility.hpp"
#include <depfile-parser.hpp>

using namespace DependencyFileParser::detail;

namespace {
    std::tuple<std::string, const char *>   fetch_token (const char *p, const char *end_p) {
        assert (p < end_p);
        assert (! isspace (*p));

        std::string tok;
        while (p < end_p) {
            if (isspace (*p)) {
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
        assert (p == end_p);
        return {std::move (tok), end_p};
    }

}
namespace DependencyFileParser {

    Result Parse (const char* src, size_t size) {
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
}
