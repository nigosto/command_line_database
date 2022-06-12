#include <fstream>
#include <exception>
#include <stdexcept>
#include "database_handler.hpp"

DatabaseHandler::DatabaseHandler() : isOpened(false) {}

void DatabaseHandler::readCommands()
{
    std::string command;
    do
    {
        std::cout << "> ";
        std::getline(std::cin, command);
        std::string space_delimiter = " ";
        std::vector<std::string> parameters{};

        size_t pos = 0;
        while ((pos = command.find(space_delimiter)) != std::string::npos)
        {
            parameters.push_back(command.substr(0, pos));
            command.erase(0, pos + space_delimiter.length());
        }
        parameters.push_back(command);
        command = parameters[0];
        parameters.erase(std::begin(parameters));

        if (command == "open" && !isOpened)
        {
            try
            {
                open(parameters[0]);
                std::cout << "Successfully opened " << parameters[0] << '\n';
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "open" && isOpened)
        {
            std::cout << "There is a file currently opened!\n";
        }
        else if (command == "help")
        {
            // TODO
        }
        else if (command == "save" && isOpened)
        {
            // TODO
        }
        else if (command == "saveas" && isOpened)
        {
            // TODO
        }
        else if (command == "close" && isOpened)
        {
            // TODO
        }
        else if (command == "exit" && isOpened)
        {
            std::cout << "You have an open file with unsaved changes, please select close or save first.\n";
            command = "";
        }
        else if (command == "import" && isOpened)
        {
            // TODO
        }
        else if (command == "showtables" && isOpened)
        {
            // TODO
        }
        else if (command == "describe" && isOpened)
        {
            // TODO
        }
        else if (command == "print" && isOpened)
        {
            // TODO
        }
        else if (command == "export" && isOpened)
        {
            // TODO
        }
        else if (command == "select" && isOpened)
        {
            // TODO
        }
        else if (command == "addcolumn" && isOpened)
        {
            // TODO
        }
        else if (command == "update" && isOpened)
        {
            // TODO
        }
        else if (command == "delete" && isOpened)
        {
            // TODO
        }
        else if (command == "insert" && isOpened)
        {
            // TODO
        }
        else if (command == "innerjoin" && isOpened)
        {
            // TODO
        }
        else if (command == "rename" && isOpened)
        {
            // TODO
        }
        else if (command == "count" && isOpened)
        {
            // TODO
        }
        else if (command == "aggregate" && isOpened)
        {
            // TODO
        }
        else if (command == "count" || command == "rename" || command == "innerjoin" || command == "insert" || command == "delete" || command == "update" || command == "addcolumn" || command == "select" || command == "export" || command == "print" || command == "describe" || command == "showtables" || command == "import" || command == "close" || command == "saveas" || command == "save" || command == "aggregate" && !isOpened)
        {
            std::cout << "There is not a file currently opened!\n";
        }
        else
        {
            if (command != "exit")
            {
                std::cout << "Invalid command! Type \"help\" to see list of commands!\n";
            }
        }
    } while (command != "exit");
}

void DatabaseHandler::open(const std::string &filename)
{
    std::ifstream is{
        filename,
        std::ios::in};

    if(!is) {
        throw std::invalid_argument("Couldn't open specified file!");
    }

    database.deserialize(is);
    is.close();
    isOpened = true;
}