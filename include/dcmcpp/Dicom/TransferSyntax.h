#pragma once

#include <string_view>

namespace dcmcpp
{

constexpr bool isExplicit(std::string_view ts)
{
    // TODO: add other explicit syntaxes
    return ts == "1.2.840.10008.1.2.1";
}

}
