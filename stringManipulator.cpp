#include <algorithm>
#include <cctype>
#include <exception>
#include <stdexcept>
#include "stringManipulator.hpp"

size_t StringManipulator::stringToInt(const std::string &str)
{
    size_t result = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if(str[i] < '0' || str[i] > '9') {
            throw std::invalid_argument("String contains characters that are not digits");
        }
        result *= 10;
        result += str[i] - '0';
    }
    return result;
}

std::string StringManipulator::trimLeft(const std::string &str)
{
    size_t start = str.find_first_not_of(' ');
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringManipulator::trimRight(const std::string &str)
{
    size_t end = str.find_last_not_of(' ');
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

void StringManipulator::toLowerCase(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
}