#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace dcmcpp
{
namespace StringUtils
{

// TODO: documentation
// TODO: use string_view intead of std::string
std::vector<std::string> split(const std::string& text, const std::string& delims);

// TODO: documentation
// TODO: use string_view instead of std::string
bool startsWith(const std::string& text, const std::string& start);

// trim from start (in place)
void ltrim(std::string& s);

// trim from end (in place)
void rtrim(std::string& s);

// trim from both ends (in place)
void trim(std::string& s);

// TODO: doc
template<typename T>
std::string intToHex(T i)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(4) << std::hex << i;
    return stream.str();
}

}
}
