#ifndef __STRING_MANIPULATOR_HPP
#define __STRING_MANIPULATOR_HPP

#include <string>

class StringManipulator
{
private:
    StringManipulator() = default;

public:
    static size_t stringToInt(const std::string &str);
    static std::string trimLeft(const std::string &str);
    static std::string trimRight(const std::string &str);
    static void toLowerCase(std::string& str);
};

#endif