#ifndef __DATABASE_HANDLER_HPP
#define __DATABASE_HANDLER_HPP

#include "database.hpp"

class DatabaseHandler {
private:
    Database database;
    std::string file;
    bool isOpened;
    void open(const std::string& filename);
public:
    DatabaseHandler();
    void readCommands();
    //TODO
};

#endif