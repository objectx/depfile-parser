//
// Copyright (c) 2019 Masashi Fujita. All rights reserved.
//
#pragma once

#include <depfile-parser.hpp>
#include <string>
#include <tuple>
#include <vector>

namespace DependencyFileParser { namespace detail {
    /// @brief Skips white spaces
    /// @param src Start of the sequence
    /// @param end_p End of the sequence
    /// @return Pointer to the non-whitespace character if found.  Otherwise return `end_p`.
    const char *skip_space (const char *src, const char *end_p);

    /// @brief Skip until EOL
    /// @param src Start of the sequence
    /// @param end_p End of the sequence
    /// @return Pointer to next to EOL character if possible.  Otherwise return `end_p`.
    const char *skip_to_eol (const char *src, const char *end_p);

    /// @brief Skips comments and spaces as possible.
    /// @param p Start of the sequence
    /// @param end_p End of the sequence
    /// @return Pointer to next to non-comment/non-whitespace character if possible.
    ///         Otherwise return `end_p`.
    const char *skip_comment_and_space (const char *p, const char *end_p);

    /// @brief Parse a dependency description.
    ///
    ///        Assumes the following inputs:<br/>
    ///        &lt;target&gt;:&lt;primary prerequisite&gt; [&lt;prerequisites...&gt;]
    /// @tparam Tokenizer_ Type of the tokenizer
    /// @param tok The tokenizer
    /// @param src Start of the sequence
    /// @param end_p End of the sequence
    /// @return Dependency information.
    /// @remarks Tokenizer_ should satisfy `std::is_invokable_r_v<const char *, const char *, const char *>`.
    template<typename Tokenizer_>
    ::DependencyFileParser::Result parse (Tokenizer_ tok, const char *src, const char *end_p) {
        src = skip_comment_and_space (src, end_p);
        if (src == end_p) {
            // Empty definitions.
            return {};
        }
        // 1st: Obtains target.
        auto const &target = tok (src, end_p);

        src = std::get<1> (target);
        if (src == end_p) {
            // Malformed dependency definition.
            return {};
        }
        std::string target_path;
        // Handle "<filepath>:" case.
        {
            auto const &p = std::get<0> (target);
            if (*std::rbegin (p) == ':') {
                // Adhered ':'
                target_path = p.substr (0, p.size () - 1);
            }
            else {
                target_path = std::get<0> (target);
                if (*src != ':') {
                    return {};  // Malformed.
                }
                src = skip_space (src + 1, end_p);
            }
        }
        if (src == end_p) {
            return {};
        }
        // 2nd: Obtains dependents.
        std::vector<std::string> deps;
        while (src < end_p) {
            auto dep = tok (src, end_p);
            deps.emplace_back (std::get<0> (dep));
            src = std::get<1> (dep);
            if (src == end_p) {
                break;
            }
        }
        return {std::move (target_path), std::move (deps)};
    }
}}  // namespace DependencyFileParser::detail
