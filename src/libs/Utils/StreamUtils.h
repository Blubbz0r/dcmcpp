#pragma once

#include <istream>

namespace dcmcpp::StreamUtils
{

/*! Returns the amount of bytes available in the given \a stream. */
std::streamoff availableBytes(std::istream& stream);

}
