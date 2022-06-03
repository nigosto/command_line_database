#ifndef __COLUMN_HPP
#define __COLUMN_HPP

#include <string>
#include <vector>

class Column {
private:
    std::vector<std::string> values;
    virtual bool validate(const std::string& value) const = 0;
public:
    Column(size_t cells = 0);
    virtual ~Column() = default;
    virtual Column* clone() const = 0;
    void insertCell(const std::string& value);
    virtual const std::string type() const = 0;
    size_t size() const { return values.size();}
    std::string& operator[](size_t index) { return values[index];}
    const std::string& operator[](size_t index) const { return values[index];}
};

#endif