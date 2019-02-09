/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */

/// @file
/// @brief Parses a dependency file emitted by clang.

#pragma once
#ifndef depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22
#define depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22 1

#include <string>
#include <vector>
#include <optional>

namespace DependencyFileParser {

    class Result final {
        std::string              target_;
        std::vector<std::string> dependents_;

    public:
        Result () = default;
        Result (const std::string& target, const std::vector<std::string>& deps)
            : target_{target}
            , dependents_{deps} {
            /* NO-OP */
        }
        Result (std::string target, std::vector<std::string>&& deps)
            : target_{std::move (target)}
            , dependents_{std::move (deps)} {
            /* NO-OP */
        }

        const std::string& target () const { return target_; }
        auto               begin () const { return dependents_.begin (); }
        auto               end () const { return dependents_.end (); }
        size_t             size () const { return dependents_.size (); }
        const std::string& operator[] (size_t idx) const {
            return dependents_[idx];
        }
    };


    /// @brief Parse supplied bytes as a UN?X Make style dependency definition.
    /// @param src The source byte sequence
    /// @param src_size # of bytes in the sequence
    /// @return Parsed dependency
    std::optional<Result> Parse (const char* src, size_t src_size);

    inline std::optional<Result>  ParseDependencyFile (const std::string& src) {
        return Parse (src.c_str (), src.size ());
    }

    /// @brief Parse supplied bytes as a NMake style dependency definition.
    /// @param src The source byte sequence
    /// @param src_size # of bytes in the sequence
    /// @return Parsed dependency
    std::optional<Result> ParseNMakeStyle (const char* src, size_t src_size);

    inline std::optional<Result>  ParseNMakeStyle (const std::string& src) {
        return ParseNMakeStyle (src.c_str (), src.size ());
    }
}

#endif /* depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22 */
