#include "column.hpp"

Column::Column(size_t cells) : values(cells) {
    for (size_t i = 0; i < cells; i++)
    {
        values[i] = "NULL";
    }   
}

void Column::insertCell(const std::string &value) {
    if(validate(value)) {
        values.push_back(value);
    }
}