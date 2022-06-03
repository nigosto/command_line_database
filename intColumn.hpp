#ifndef __INT_COLUMN_HPP
#define __INT_COLUMN_HPP

#include "column.hpp"
#include "serializeable.hpp"

class IntColumn : public Column, public ISerializeable
{
private:
    bool validate(const std::string &value) const final;

public:
    IntColumn(size_t cells = 0);
    IntColumn *clone() const final;
    const std::string type() const final { return "integer"; }
    std::ostream &serialize(std::ostream &os) const final;
    std::istream &deserialize(std::istream &is) final;
    int valueAsNumber(size_t index);
};

#endif