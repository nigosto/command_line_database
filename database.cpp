#include "database.hpp"
#include <algorithm>
#include <exception>
#include <stdexcept>

Table *Database::find(const std::string &name)
{
    std::vector<Table>::iterator result = std::find_if(std::begin(tables), std::end(tables), [name](const Table &table)
                                                       { return table.getName() == name; });
    if (result != std::end(tables))
    {
        return &(*(result));
    }
    return nullptr;
}

void Database::import(const std::string &filename)
{
    Table table(filename);
    table.deserialize();

    // std::vector<Table>::iterator result = std::find_if(std::begin(tables), std::end(tables), [table](const Table &t)
    //                                                    { return t.getName() == table.getName(); });
    // if (result != std::end(tables))
    // {
    //     throw std::runtime_error("A table with this name already exists!");
    // }

    for (size_t i = 0; i < tables.size(); i++)
    {
        if (tables[i].getName() == table.getName())
        {
            throw std::runtime_error("A table with this name already exists!");
        }
    }

    tables.push_back(table);
}

std::ostream &Database::serialize(std::ostream &os) const
{
    os << tables.size() << '\n';
    for (size_t i = 0; i < tables.size(); i++)
    {
        os << tables[i].getName() << '\n'
           << tables[i].getFilename() << '\n';
    }
    return os;
}

std::istream &Database::deserialize(std::istream &is)
{
    if (is.peek() != std::istream::traits_type::eof())
    {
        size_t size = 0;
        is >> size;

        is.ignore();
        for (size_t i = 0; i < size; i++)
        {
            std::string name;
            std::getline(is, name);
            std::string filename;
            std::getline(is, filename);
            Table t(filename, name);
            tables.push_back(t);
            tables[i].deserialize();
        }
    }
    return is;
}
