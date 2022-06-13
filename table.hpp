#ifndef __TABLE_HPP
#define __TABLE_HPP

#include "column.hpp"
#include "serializeableImplicitly.hpp"

class Table : public ISerializeableImplicitly
{
private:
    std::vector<Column *> columns;
    std::string name;
    std::string filename;
    size_t rows_count;
    size_t columns_count;

public:
    Table(const std::string &_filename="", const std::string &_name = "");
    //Table(std::string &&_filename, std::string &&_name);
    Table(const Table &other);
    //Table(Table &&other);
    ~Table();
    Table &operator=(const Table &other);
    //Table &operator=(Table &&other);
    void swap(Table &other);
    void addColumn(const std::string &_name, const std::string &_type);
    //std::string *find(size_t columnIndex, const std::string &value);
    void insertRow(const std::vector<std::string>& values);
    void rename(const std::string &_name);
    void serialize() const final;
    void deserialize() final;
    const std::string& getName() const {return name;}
    const std::string& getFilename() const {return filename;}
    size_t getRows() const { return rows_count; }
    size_t getColumns() const {return columns_count;}
    Column* operator[](size_t index) {return columns[index];}
    void changeFilename(const std::string& file) { filename = file; }
    void remove(size_t rowIndex);
    friend Table innerJoin(const Table& first, size_t firstColumn, const Table& second, size_t secondColumn);

    //using during development only
    void print() const {
        for (size_t i = 0; i < columns_count; i++)
        {
            std::cout<<columns[i]->getName()<< ' ';
        }
        std::cout<<'\n';
        for (size_t i = 0; i < rows_count; i++)
        {
            for (size_t j = 0; j < columns_count; j++)
            {
                std::cout<<(*columns[j])[i]<< ' ';
            }
            std::cout<<'\n';
        }
        
    }
};

#endif