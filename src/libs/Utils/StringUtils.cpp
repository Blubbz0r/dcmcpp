#include "StringUtils.h"

#include <algorithm>
#include <cctype>
#include <locale>

namespace dcmcpp
{
namespace StringUtils
{

std::vector<std::string> split(const std::string& text, const std::string& delims)
{
    std::vector<std::string> tokens;

    if (text.empty())
        return tokens;

    std::size_t start = 0, end = 0;
    while ((end = text.find(delims, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(text.substr(start));
    return tokens;
}

bool startsWith(const std::string& text, const std::string& start)
{
    return start.length() <= text.length()
        && std::equal(start.begin(), start.end(), text.begin());
}

void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

}
}

