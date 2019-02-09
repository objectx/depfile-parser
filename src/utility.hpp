//
// Copyright (c) 2019  Polyphony Digital Inc.  All rights reserved.
//
#pragma once
#ifndef utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9
#define utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9

namespace DependencyFileParser::detail {
    const char* skip_space (const char* src, const char* end_p);
    const char* skip_to_eol (const char* src, const char* end_p);
} /* namespace DependencyFileParser::detail */

#endif /* utility_hpp__779FE0AC_DF0D_4F24_9312_AB0C307D20B9 */
