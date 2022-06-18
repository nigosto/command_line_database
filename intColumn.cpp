#include "intColumn.hpp"

bool IntColumn::validate(const std::string &value) const
{
    if (value[0] == '+' || value[0] == '-' || (value[0] >= '0' && value[0] <= '9'))
    {
        for (size_t i = 1; i < value.length(); i++)
        {
            if (value[i] < '0' || value[i] > '9')
            {
                return false;
            }
        }
    }
    return true;
}

IntColumn::IntColumn(const std::string& _name, size_t cells) : Column(_name,cells) {}

IntColumn *IntColumn::clone() const
{
    return new IntColumn(*this);
}

int IntColumn::valueAsNumber(size_t index) const
{
    if ((*this)[index] == "NULL")
    {
        return 0;
    }
    int result = 0;
    if ((*this)[index][0] == '-' || (*this)[index][0] == '+')
    {
        for (size_t i = 1; i < (*this)[index].length(); i++)
        {
            result *= 10;
            result += (*this)[index][i] - '0';
        }
        if ((*this)[index][0] == '-')
        {
            result = -result;
        }
    }
    else
    {
        for (size_t i = 0; i < (*this)[index].length(); i++)
        {
            result *= 10;
            result += (*this)[index][i] - '0';
        }
    }
    return result;
}
