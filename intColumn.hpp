#ifndef __INT_COLUMN_HPP
#define __INT_COLUMN_HPP

#include "column.hpp"

class IntColumn : public Column
{
private:
    bool validate(const std::string &value) const final;

public:
    IntColumn(const std::string& _name, size_t cells = 0);
    IntColumn *clone() const final;
    const std::string type() const final { return "integer"; }
    int valueAsNumber(size_t index);
};

#endif