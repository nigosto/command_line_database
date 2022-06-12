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
public:
    DatabaseHandler();
    void readCommands();
    //TODO
};

#endif