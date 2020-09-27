//
// Copyright (c) 2019 Masashi Fujita. All rights reserved.
//
#pragma once

#include <depfile-parser.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace DependencyFileParser::detail {
    using tokenizer_result_t = std::tuple<std::string, std::string_view>;

    std::string_view skip_space (std::string_view s);

    /// @brief Skip until EOL
    std::string_view skip_to_eol (std::string_view s);

    /// @brief Skips comments and spaces as possible.
    std::string_view skip_comment_and_space (std::string_view s);

    /// @brief Parse a dependency description.
    ///
    ///        Assumes the following inputs:<br/>
    ///        &lt;target&gt;:&lt;primary prerequisite&gt; [&lt;prerequisites...&gt;]
    /// @tparam Tokenizer_ Type of the tokenizer
    /// @param tok The tokenizer
    /// @param src Start of the sequence
    /// @param end_p End of the sequence
    /// @return Dependency information.
    template<typename Tokenizer_>
    std::enable_if_t<std::is_invocable_r_v<tokenizer_result_t, Tokenizer_, std::string_view>, ::DependencyFileParser::Result>
    parse (Tokenizer_ tok, std::string_view src) {
        src = skip_comment_and_space (src);
        if (src.empty()) {
            return {};
        }
        // 1st: Obtains target.
        auto const &target = tok (src);

        src = std::get<1> (target);
        if (src.empty()) {
            // Malformed dependency definition.
            return {};
        }
        std::string_view target_path;
        // Handle "<filepath>:" case.
        {
            std::string_view p {std::get<0> (target)};
            if (*std::rbegin (p) == ':') {
                // Adhered ':'
                target_path = p.substr (0, p.size() - 1);
            }
            else {
                target_path = std::get<0> (target);
                if (src[0] != ':') {
                    return {};  // Malformed.
                }
                src = skip_space (src.substr (1));
            }
        }
        if (src.empty ()) {
            return {};
        }
        // 2nd: Obtains dependents.
        std::vector<std::string> deps;
        while (! src.empty ()) {
            auto dep = tok (src);
            deps.emplace_back (std::get<0> (dep));
            src = std::get<1> (dep);
        }
        return {target_path, std::move (deps)};
    }
}  // namespace DependencyFileParser::detail
