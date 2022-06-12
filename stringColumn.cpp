#include "stringColumn.hpp"

bool StringColumn::validate(const std::string &value) const {
    if(value[0] != '\"' || value[value.length()-1] != '\"') {
        return false;
    }
    for (size_t i = 1; i < value.length()-1; i++)
    {
        if(value[i] == '"' && value[i-1] != '\\') {
            return false;
        } 
        
        if(value[i] == '\\' && value[i+1] != '\\' && value[i+1] != '"') {
            return false;
        } else {
            i++;
        }
    }
    return true;
}

StringColumn::StringColumn(const std::string& _name, size_t cells) : Column(_name,cells) {}

StringColumn *StringColumn::clone() const {
    return new StringColumn(*this);
}