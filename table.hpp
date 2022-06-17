#ifndef __TABLE_HPP
#define __TABLE_HPP

#include "column.hpp"

class Table
{
private:
    std::vector<Column *> columns;
    std::string name;
    std::string filename;
    size_t rows_count;
    size_t columns_count;

public:
    Table(const std::string &_filename = "", const std::string &_name = "");
    Table(const Table &other);
    ~Table();
    Table &operator=(const Table &other);
    void swap(Table &other);
    void addColumn(const std::string &_name, const std::string &_type);
    void insertRow(const std::vector<std::string> &values);
    void rename(const std::string &_name);
    void serialize(bool recovery = false) const;
    void deserialize(bool recovery = false);
    const std::string &getName() const { return name; }
    const std::string &getFilename() const { return filename; }
    size_t getRows() const { return rows_count; }
    size_t getColumns() const { return columns_count; }
    Column *operator[](size_t index);
    void changeFilename(const std::string &file) { filename = file; }
    void remove(size_t rowIndex);
    friend Table innerJoin(const Table &first, size_t firstColumn, const Table &second, size_t secondColumn);
};

#endif