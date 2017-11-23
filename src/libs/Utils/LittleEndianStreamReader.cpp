#include "LittleEndianStreamReader.h"

namespace dcmcpp::LittleEndian
{

std::string readString(std::istream& stream, uint32_t size)
{
    const auto availableBytes = StreamUtils::availableBytes(stream);
    if (availableBytes < size)
    {
        // TODO: proper exception
        throw std::runtime_error(
            "Not enough bytes in stream to read the requested string (available: " +
            std::to_string(availableBytes) + "; requested: " + std::to_string(size) + ")");
    }

    std::string s;
    s.resize(size);
    stream.read(s.data(), size);
    return s;
}

}
