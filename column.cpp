#include "column.hpp"

Column::Column(const std::string& _name, size_t cells) : values(cells), name(_name) {
    for (size_t i = 0; i < cells; i++)
    {
        values[i] = "NULL";
    }   
}

void Column::push_back(const std::string &value) {
    if(validate(value)) {
        values.push_back(value);
    }
}

std::ostream &Column::serialize(std::ostream &os) const
{
    os << getName() << '\n' << type() << '\n' << size()<<'\n';
    for (size_t i = 0; i < size(); i++)
    {
        os << (*this)[i] << '\n';    
    }
    return os;
}

std::istream &Column::deserialize(std::istream &is)
{
    size_t dataSize;
    is >> dataSize;
    for (size_t i = 0; i < dataSize; i++)
    {
        std::string str;
        std::getline(is, str);
        push_back(str);
    }
    return is;
}