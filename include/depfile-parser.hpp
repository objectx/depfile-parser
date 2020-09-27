/*
 * Copyright (c) 2019 Masashi Fujita. All rights reserved.
 */

/// @file
/// @brief Parses a dependency file emitted by clang.

#pragma once

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace DependencyFileParser {

    /**
     * @brief Represents the dependency result.
     */
    class Result final {
        std::string              target_;
        std::vector<std::string> prerequisites_;

    public:
        ~Result ()              = default;
        Result ()               = default;
        Result (const Result &) = default;
        Result (Result &&)      = default;
        Result (std::string_view target, std::vector<std::string> prereq)
                : target_ {target.begin (), target.end ()}
                , prerequisites_ {std::move (prereq)} {
            /* NO-OP */
        }
        Result &           operator= (const Result &) = default;
        Result &           operator= (Result &&) = default;
        [[nodiscard]] bool is_valid () const noexcept { return ! target_.empty (); }
        explicit           operator bool () const noexcept { return is_valid (); }

        [[nodiscard]] const std::string &             target () const { return target_; }
        [[nodiscard]] const std::vector<std::string> &prerequisites () const { return prerequisites_; }
    };

    /// @brief Parse supplied bytes as a UN?X Make style dependency definition.
    /// @param src The source byte sequence
    /// @param size # of bytes in the sequence
    /// @return Parsed dependency
    Result Parse (std::string_view src);

    /// @brief Parse supplied bytes as a NMake style dependency definition.
    /// @param src The source byte sequence
    /// @param size # of bytes in the sequence
    /// @return Parsed dependency
    Result ParseNMakeStyle (std::string_view src);
}  // namespace DependencyFileParser
