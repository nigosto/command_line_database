#ifndef __DATABASE_HANDLER_HPP
#define __DATABASE_HANDLER_HPP

#include "database.hpp"

class DatabaseHandler {
private:
    Database database;
    std::string file;
    bool isOpened;

    void open(const std::string& filename);
    void help() const noexcept;
    void save();
    void saveas(const std::string& filename);
    void close();
    void showtables() const;
    void describe(const std::string& name);
    void print(const std::string& name);
    void exportTable(const std::string& name, const std::string& filename);
    void select(size_t column, const std::string& value, const std::string& table);
    void addcolumn(const std::string& table, const std::string& column, const std::string& type);
    void update(const std::string& table, size_t searchColumn, const std::string& searchValue, size_t targetColumn, const std::string& targetValue);
    void deleteRows(const std::string& table, size_t column, const std::string& value);
    void insert(const std::vector<std::string>& parameters);
    void rename(const std::string& oldName, const std::string& newName);
    void count(const std::string& table, size_t column, const std::string& value);
    void innerjoin(const std::string& table1, size_t column1, const std::string& table2, size_t column2);
    void aggregate(const std::string& table, size_t searchColumn, const std::string& searchValue, size_t targetColumn, const std::string& operation);
public:
    DatabaseHandler();
    void readCommands();
};

#endif