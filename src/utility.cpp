//
// Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
//

#include "utility.hpp"

#include <cctype>

namespace DependencyFileParser {
    namespace detail {

        const char* skip_space (const char* p, const char* end_p) {

            while (p < end_p) {
                if (*p == '\\') {
                    // Maybe a continued line.
                    ++p;
                    if (end_p <= p) {
                        // Stray '\\'.
                        return p - 1;
                    }
                    // Checks \\\n case.
                    if (*p == '\n') {
                        ++p;
                        continue;
                    }
                    // Checks \\\r\n case (for windows).
                    if (*p == '\r') {
                        ++p;
                        if (end_p <= p) {
                            // "\\\r" -> considered as stray '\\'
                            return p - 2;
                        }
                        if (*p == '\n') {
                            ++p;
                            continue;
                        }
                    }
                    // At this point, *p points next to non EOL character.
                    return p - 1; // Points '\\'
                }
                if (!isspace (*p)) {
                    return p;
                }
                ++p;
            }
            return end_p;
        }

        const char* skip_to_eol (const char* p, const char* end_p) {
            while (p < end_p) {
                if (*p == '\n') {
                    return p + 1;
                }
                ++p;
            }
            return end_p;
        }

        const char* skip_comment_and_space (const char* p, const char* end_p) {
            while (p < end_p) {
                auto q = skip_space (p, end_p);
                if (q == end_p) {
                    return end_p;
                }
                if (*q != '#') {
                    return q;
                }
                // *q == '#'.  Skip to EOL.
                p = skip_to_eol (q + 1, end_p);
                if (p == end_p) {
                    return end_p;
                }
            }
            return end_p;
        }
    } // namespace detail
} // namespace DependencyFileParser
