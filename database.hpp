#ifndef __DATABASE_HPP
#define __DATABASE_HPP

#include "table.hpp"
#include "serializeable.hpp"

class Database : public ISerializeable {
private:
    std::vector<Table> tables;

public:
    Database() : tables(0) {}
    Table* find(const std::string& name);
    void import(const std::string& filename);
    std::ostream &serialize(std::ostream &os = std::cout) const final;
    std::istream &deserialize(std::istream &is = std::cin) final;
    size_t size() const {return tables.size(); }
    const Table& operator[](size_t index) const;
};

#endif