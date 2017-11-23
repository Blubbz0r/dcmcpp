#pragma once

#include <istream>

namespace dcmcpp::StreamUtils
{

/*! Returns the amount of bytes available in the given \a stream. */
size_t availableBytes(std::istream& stream);

}
