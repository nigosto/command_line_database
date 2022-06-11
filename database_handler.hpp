#ifndef __DATABASE_HANDLER_HPP
#define __DATABASE_HANDLER_HPP

#include "database.hpp"

class DatabaseHandler {
private:
    Database database;
    std::string file;
public:
    void readCommands();
    //TODO
};

#endif