#ifndef __COLUMN_HPP
#define __COLUMN_HPP

#include <string>
#include <vector>
#include "serializeableExplicitly.hpp"

class Column : public ISerializeableExplicitly
{
private:
    std::string name;
    std::vector<std::string> values;
    virtual bool validate(const std::string &value) const = 0;

public:
    Column(const std::string& _name = "", size_t cells = 0);
    virtual ~Column() = default;
    virtual Column *clone() const = 0;
    void push_back(const std::string &value);
    virtual const std::string type() const = 0;
    const std::string& getName() const {return name;}
    size_t size() const { return values.size(); }
    std::string &operator[](size_t index) { return values[index]; }
    const std::string &operator[](size_t index) const { return values[index]; }
    std::ostream &serialize(std::ostream &os = std::cout) const override;
    std::istream &deserialize(std::istream &is = std::cin) override;
};

#endif