#include "Tag.h"

#include "Utils/StringUtils.h"

#include <algorithm>

namespace dcmcpp
{

Tag tagFromString(std::string s)
{
    s.erase(std::remove(s.begin(), s.end(), '('), s.end());
    s.erase(std::remove(s.begin(), s.end(), ')'), s.end());
    auto parts = StringUtils::split(s, ",");

    try
    {
        return Tag{static_cast<uint16_t>(std::stoi(parts[0], 0, 16)),
                   static_cast<uint16_t>(std::stoi(parts[1], 0, 16))};
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Unable to convert '" + s + "' to Tag: " + e.what());
    }
}

std::string tagToString(const Tag& tag)
{
    return "(" + StringUtils::intToHex(tag.group) + "," + StringUtils::intToHex(tag.element) + ")";
}

}
