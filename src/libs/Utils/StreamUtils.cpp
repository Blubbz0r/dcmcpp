#include "StreamUtils.h"

namespace dcmcpp::StreamUtils
{

size_t availableBytes(std::istream& stream)
{
    const auto currentPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    const auto availableBytes = stream.tellg() - currentPos;
    stream.seekg(currentPos);
    return availableBytes;
}

}
