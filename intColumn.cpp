#include "intColumn.hpp"

bool IntColumn::validate(const std::string &value) const
{
    return true;
    // TODO
}

IntColumn::IntColumn(size_t cells) : Column(cells) {}

IntColumn *IntColumn::clone() const
{
    return new IntColumn(*this);
}

std::ostream &IntColumn::serialize(std::ostream &os) const
{
    // TODO
}

std::istream &IntColumn::deserialize(std::istream &is)
{
    // TODO
}

int IntColumn::valueAsNumber(size_t index)
{
    if((*this)[index] == "NULL") {
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
