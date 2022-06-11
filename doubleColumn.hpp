#ifndef __DOUBLE_COLUMN_HPP
#define __DOUBLE_COLUMN_HPP

#include "column.hpp"

class DoubleColumn : public Column
{
private:
    bool validate(const std::string &value) const final;

public:
    DoubleColumn(const std::string& _name = "", size_t cells = 0);
    DoubleColumn *clone() const final;
    const std::string type() const final { return "double"; }
    double valueAsNumber(size_t index);
};

#endif