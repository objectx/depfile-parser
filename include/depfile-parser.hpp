/*
 * Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
 */

/// @file
/// @brief Parses a dependency file emitted by clang.

#pragma once
#ifndef depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22
#define depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22    1

#include <string>
#include <vector>

class DependencyResult final {
    std::string   target_;
    std::vector<std::string>  dependents_;
    bool valid_ = false;
public:
    DependencyResult () = default;
    DependencyResult (const std::string &target, const std::vector<std::string> &deps)
        : target_ { target }
        , dependents_ { deps }
        , valid_ { true } {
        /* NO-OP */
    }
    DependencyResult (std::string target, std::vector<std::string> &&deps)
        : target_ { std::move (target) }
        , dependents_ { std::move (deps) }
        , valid_ { true } {
        /* NO-OP */
    }

    const std::string & target () const { return target_ ; }
    auto begin () const { return dependents_.begin () ; }
    auto end () const { return dependents_.end () ; }
    size_t size () const { return dependents_.size () ; }
    const std::string & operator [] (size_t idx) const {
        return dependents_ [idx];
    }
    bool is_valid () const { return valid_ ; }
    explicit operator bool () const { return is_valid () ; }
};

/// @brief Parse supplied bytes as a UN?X Make style dependency definition.
DependencyResult ParseDependencyFile (const char *src, size_t src_size);

inline DependencyResult ParseDependencyFile (const std::string &src) {
    return ParseDependencyFile (src.c_str (), src.size ());
}


/// @brief Parse supplied bytes as a NMake style dependency definition.
DependencyResult ParseNMakeStyleDependencyFile (const char *src, size_t src_size);

inline DependencyResult ParseNMakeStyleDependencyFile (const std::string &src) {
    return ParseNMakeStyleDependencyFile (src.c_str (), src.size ());
}
#endif  /* depfile_parser_hpp__56a43180cc954e9cba64a0fb63127d22 */
