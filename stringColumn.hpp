#ifndef __STRING_COLUMN_HPP
#define __STRING_COLUMN_HPP

#include "column.hpp"
#include "serializeable.hpp"

class StringColumn : public Column, public ISerializeable
{
private:
    bool validate(const std::string &value) const final;

public:
    StringColumn(size_t cells = 0);
    StringColumn *clone() const final;
    const std::string type() const final { return "string"; }
    std::ostream &serialize(std::ostream &os) const final;
    std::istream &deserialize(std::istream &is) final;
};

#endif