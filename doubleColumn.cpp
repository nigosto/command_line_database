#include "doubleColumn.hpp"

bool DoubleColumn::validate(const std::string &value) const
{
    if (value[0] == '+' || value[0] == '-' || (value[0] >= '0' && value[0] <= '9'))
    {
        int dotCount = 0;
        for (size_t i = 1; i < value.length(); i++)
        {
            if (value[i] == '.')
            {
                dotCount++;
                if (dotCount > 1)
                {
                    return false;
                }
            }
            else if (value[i] < '0' || value[i] > '9')
            {
                return false;
            }
        }
    }
    return true;
}

DoubleColumn::DoubleColumn(const std::string &_name, size_t cells) : Column(_name, cells) {}

DoubleColumn *DoubleColumn::clone() const
{
    return new DoubleColumn(*this);
}

double DoubleColumn::valueAsNumber(size_t index)
{
    if ((*this)[index] == "NULL")
    {
        return 0;
    }
    double result = 0;
    if ((*this)[index][0] == '-' || (*this)[index][0] == '+')
    {
        long multiplier = 10;
        unsigned i = 1;
        while ((*this)[index][i] && (*this)[index][i] != '.')
        {
            result *= 10;
            result += (*this)[index][i++] - '0';
        }
        i++;
        while ((*this)[index][i])
        {
            double digit = static_cast<double>((*this)[index][i++] - '0');
            result += digit / multiplier;
            multiplier *= 10;
        }

        if ((*this)[index][0] == '-')
        {
            result = -result;
        }
    }
    else
    {
        long multiplier = 10;
        unsigned i = 0;
        while ((*this)[index][i] && (*this)[index][i] != '.')
        {
            result *= 10;
            result += (*this)[index][i++] - '0';
        }
        i++;
        while ((*this)[index][i])
        {
            double digit = static_cast<double>((*this)[index][i++] - '0');
            result += digit / multiplier;
            multiplier *= 10;
        }
    }
    return result;
}
