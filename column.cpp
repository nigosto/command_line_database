#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"
#include "column.hpp"

Column::Column(const std::string &_name, size_t cells) : values(cells), name(_name)
{
    for (size_t i = 0; i < cells; i++)
    {
        values[i] = "NULL";
    }
}

void Column::push_back(const std::string &value)
{
    if (validate(value) || value == "NULL")
    {
        values.push_back(value);
    }
}

const std::string &Column::operator[](size_t index) const
{
    if (index < 0 || index > values.size() - 1)
    {
        throw std::runtime_error("Invalid index!");
    }

    return values[index];
}

std::ostream &Column::serialize(std::ostream &os) const
{
    os << type() << '\n'
       << getName() << '\n'
       << size() << '\n';
    for (size_t i = 0; i < size(); i++)
    {
        os << (*this)[i] << '\n';
    }
    return os;
}

std::istream &Column::deserialize(std::istream &is)
{
    std::string _name;
    std::getline(is, _name);
    name = _name;
    size_t dataSize;
    is >> dataSize;
    is.ignore();
    for (size_t i = 0; i < dataSize; i++)
    {
        std::string str;
        std::getline(is, str);
        push_back(str);
    }
    return is;
}

void Column::remove(size_t index)
{
    values.erase(std::begin(values) + index);
}

Column *Column::createColumn(const std::string &_type, const std::string& _name, size_t _cells)
{
    if(_type == "integer") {
        return new IntColumn(_name, _cells);
    }
    else if(_type == "double") {
        return new DoubleColumn(_name, _cells);
    }
    else if(_type == "string") {
        return new StringColumn(_name, _cells);
    }
    else {
        return nullptr;
    }
}
