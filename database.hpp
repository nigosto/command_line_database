#ifndef __DATABASE_HPP
#define __DATABASE_HPP

#include "table.hpp"
#include "serializeableExplicitly.hpp"

class Database : public ISerializeableExplicitly {
private:
    std::vector<Table> tables;

public:
    Database() : tables(0) {}
    Table* find(const std::string& name);
    void import(const std::string& filename);
    std::ostream &serialize(std::ostream &os = std::cout) const final;
    std::istream &deserialize(std::istream &is = std::cin) final;

    //using during development only
    void print() const {
        for (auto &&i : tables)
        {
            i.print();
        }
        
    }
};

#endif