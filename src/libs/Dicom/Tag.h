#pragma once

#include <cstdint>
#include <string>

namespace dcmcpp
{

// TODO: doc
struct Tag
{
    constexpr Tag() : group(0), element(0) {}
    constexpr Tag(uint16_t g, uint16_t e) : group(g), element(e) {}

    uint16_t group;
    uint16_t element;
};

// TODO: doc
Tag tagFromString(std::string s);
// TODO: doc
std::string tagToString(const Tag& tag);

// TODO: doc
constexpr bool isMetaInfo(const Tag& tag)
{
    return tag.group == 0x0002;
}

// TODO: provide other operators?
constexpr bool operator==(const Tag& lhs, const Tag& rhs)
{
    return lhs.group == rhs.group && lhs.element == rhs.element;
}

constexpr bool operator!=(const Tag& lhs, const Tag& rhs)
{
    return !(lhs == rhs);
}

}
