#include <iostream>
#include <fstream>
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"
#include "table.hpp"
#include "database.hpp"
#include "database_handler.hpp"

int main()
{
    DatabaseHandler::getInstance().readCommands();

    return 0;
}