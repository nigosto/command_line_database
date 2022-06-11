#include "doubleColumn.hpp"

bool DoubleColumn::validate(const std::string &value) const {
//TODO
}

DoubleColumn::DoubleColumn(const std::string& _name, size_t cells) : Column(_name,cells) {}

DoubleColumn *DoubleColumn::clone() const {
    return new DoubleColumn(*this);
}

double DoubleColumn::valueAsNumber(size_t index) {
//TODO
}
