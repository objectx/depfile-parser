//
// Copyright (c) 2019 Masashi Fujita. All rights reserved.
//

#include "utility.hpp"

#include <cctype>

namespace DependencyFileParser::detail {

    std::string_view skip_space (std::string_view s) {
        size_t i = 0;
        size_t len = s.size();
        while (i < len) {
            if (s[i] == '\\') {
                // Maybe a continued line.
                ++i;
                if (len <= i) {
                    // Stray '\\'.
                    return s.substr(i - 1);
                }
                // Checks \\\n case.
                if (s[i] == '\n') {
                    ++i;
                    continue;
                }
                // Checks \\\r\n case (for windows).
                if (s[i] == '\r') {
                    ++i;
                    if (len <= i) {
                        // "\\\r" -> considered as stray '\\'
                        return s.substr (i - 2);
                    }
                    if (s[i] == '\n') {
                        ++i;
                        continue;
                    }
                }
                // At this point, *p points next to non EOL character.
                return s.substr (i - 1);  // Points '\\'
            }
            if (isspace (s[i]) == 0) {
                return s.substr (i);
            }
            ++i;
        }
        return s.substr(len);
    }

    std::string_view skip_to_eol (std::string_view s) {
        size_t i = 0;
        size_t len = s.size();
        while (i < len) {
            if (s[i] == '\n') {
                return s.substr (i + 1);
            }
            ++i;
        }
        return s.substr (len);
    }

    std::string_view skip_comment_and_space (std::string_view s) {
        while (! s.empty()) {
            s = skip_space(s);
            if (! s.empty()) {
                if (s[0] != '#') {
                    return s;
                }
                // q[0] == '#'.  Skip to EOL.
                s = skip_to_eol (s.substr (1));
            }
        }
        return s;
    }
}  // namespace DependencyFileParser::detail
