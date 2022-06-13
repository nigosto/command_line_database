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
    void describe(const std::string& name);
    void print(const std::string& name);
    void exportTable(const std::string& name, const std::string& filename);
    void select(size_t column, const std::string& value, const std::string& table);
    void addcolumn(const std::string& table, const std::string& column, const std::string& type);
public:
    DatabaseHandler();
    void readCommands();
    //TODO
};

#endif