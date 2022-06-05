#include "stringColumn.hpp"

bool StringColumn::validate(const std::string &value) const {
//TODO
}

StringColumn::StringColumn(const std::string& _name, size_t cells) : Column(_name,cells) {}

StringColumn *StringColumn::clone() const {
//TODO
}