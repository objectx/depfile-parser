//
// Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
//
#pragma once
#ifndef utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9
#define utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9

namespace DependencyFileParser {
    namespace detail {
        /// @brief Skips white spaces
        /// @param src Start of the sequence
        /// @param end_p End of the sequence
        /// @return Pointer to the non-whitespace character if found.  Otherwise return `end_p`.
        const char* skip_space (const char* src, const char* end_p);

        /// @brief Skip until EOL
        /// @param src Start of the sequence
        /// @param end_p End of the sequence
        /// @return Pointer to next to EOL character if possible.  Otherwise return `end_p`.
        const char* skip_to_eol (const char* src, const char* end_p);

        /// @brief Skips comments and spaces as possible.
        /// @param p Start of the sequence
        /// @param end_p End of the sequence
        /// @return Pointer to next to non-comment/non-whitespace character if possible.
        ///         Otherwise return `end_p`.
        const char* skip_comment_and_space (const char* p, const char* end_p);

    } // namespace detail
} // namespace DependencyFileParser

#endif /* utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9 */
