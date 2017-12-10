#pragma once

#include <string>

namespace dcmcpp
{

/*!
    A character string that may be padde with leading and/or trailing spaces.
    The character code 05CH (the BACKSLASH "\" in ISO-IR 6) shall not be present, as it is used as
    the delimiter between values for multiple data elements.
    The string shall not have Control Characters except ESC.
*/

using ShortString = std::string;

}
