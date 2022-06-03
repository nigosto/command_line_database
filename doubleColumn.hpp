#ifndef __DOUBLE_COLUMN_HPP
#define __DOUBLE_COLUMN_HPP

#include "column.hpp"
#include "serializeable.hpp"

class DoubleColumn : public Column, public ISerializeable
{
private:
    bool validate(const std::string &value) const final;

public:
    DoubleColumn(size_t cells = 0);
    DoubleColumn *clone() const final;
    const std::string type() const final { return "double"; }
    std::ostream &serialize(std::ostream &os) const final;
    std::istream &deserialize(std::istream &is) final;
};

#endif