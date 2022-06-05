#ifndef __STRING_COLUMN_HPP
#define __STRING_COLUMN_HPP

#include "column.hpp"

class StringColumn : public Column
{
private:
    bool validate(const std::string &value) const final;

public:
    StringColumn(const std::string& _name, size_t cells = 0);
    StringColumn *clone() const final;
    const std::string type() const final { return "string"; }
};

#endif