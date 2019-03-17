/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */

/// @file
/// @brief Parses a dependency file emitted by clang.

#pragma once
#ifndef depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22
#define depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22 1

#include <memory>
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
        ~Result ()             = default;
        Result ()              = default;
        Result (const Result&) = default;
        Result (Result&&)      = default;
        Result (std::string target, std::vector<std::string> prereq)
            : target_{std::move (target)}
            , prerequisites_{std::move (prereq)} {
            /* NO-OP */
        }

        bool     is_valid () const noexcept { return !target_.empty (); }
        explicit operator bool () const noexcept { return is_valid (); }

        const std::string&              target () const { return target_; }
        const std::vector<std::string>& prerequisites () const { return prerequisites_; }
    };

    /// @brief Parse supplied bytes as a UN?X Make style dependency definition.
    /// @param src The source byte sequence
    /// @param size # of bytes in the sequence
    /// @return Parsed dependency
    Result Parse (const char* src, size_t size);

    inline Result Parse (const std::string& src) {
        return Parse (src.c_str (), src.size ());
    }

    /// @brief Parse supplied bytes as a NMake style dependency definition.
    /// @param src The source byte sequence
    /// @param size # of bytes in the sequence
    /// @return Parsed dependency
    Result ParseNMakeStyle (const char* src, size_t size);

    inline Result ParseNMakeStyle (const std::string& src) {
        return ParseNMakeStyle (src.c_str (), src.size ());
    }
} // namespace DependencyFileParser

#endif /* depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22 */
