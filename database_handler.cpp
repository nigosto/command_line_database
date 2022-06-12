#include <fstream>
#include <exception>
#include <stdexcept>
#include <iomanip>
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
            help();
        }
        else if (command == "save" && isOpened)
        {
            try
            {
                save();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
                throw;
            }
        }
        else if (command == "saveas" && isOpened)
        {
            try
            {
                saveas(parameters[0]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "close" && isOpened)
        {
            close();
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
            else {
                std::cout<<"Exiting program...\n";
            }
        }
    } while (command != "exit");
}

void DatabaseHandler::open(const std::string &filename)
{
    std::ifstream is{
        filename,
        std::ios::in};

    if (!is)
    {
        throw std::invalid_argument("Couldn't open specified file!");
    }

    database.deserialize(is);
    is.close();
    isOpened = true;
    file = filename;
    std::cout << "Successfully opened " << filename << '\n';
}

void DatabaseHandler::help() const noexcept
{
    std::cout << "The following commands are supported: \n";
    std::cout << std::setw(90) << std::left << "open <file>"
              << "opens <file>" << std::endl;
    std::cout << std::setw(90) << "close"
              << "closes currently opened file" << std::endl;
    std::cout << std::setw(90) << "save"
              << "saves the currently open file" << std::endl;
    std::cout << std::setw(90) << "saveas <file>"
              << "saves the currently open file in <file>" << std::endl;
    std::cout << std::setw(90) << "help"
              << "prints this information" << std::endl;
    std::cout << std::setw(90) << "exit"
              << "exits the program" << std::endl;
    std::cout << std::setw(90) << "import <file name>"
              << "imports table to database from <file name>" << std::endl;
    std::cout << std::setw(90) << "showtables"
              << "shows list of all table names in database" << std::endl;
    std::cout << std::setw(90) << "describe <name>"
              << "shows list of types of all columns in table <name>" << std::endl;
    std::cout << std::setw(90) << "print <name>"
              << "prints all rows of table <name>" << std::endl;
    std::cout << std::setw(90) << "export <name> <file name>"
              << "exports table <name> to file <file name>" << std::endl;
    std::cout << std::setw(90) << "select <column-n> <value> <table name>"
              << "prints all rows of table <table name> that contain <value> in column <column-n>" << std::endl;
    std::cout << std::setw(90) << "addcolumn <table name> <column name> <column type>"
              << "adds column <column name> of type <column type> to table <table name> with empty rows" << std::endl;
    std::cout << std::setw(90) << "update <table name> <search column n> <search value> <target column n> <target value>"
              << "for every row in table <table name> with value <search column value> in column <search column n> changes value in column <target column n> with <target value>" << std::endl;
    std::cout << std::setw(90) << "delete <table name> <search column n> <search value>"
              << "deletes every row in table <table name> that has value <search value> in column <search column n>" << std::endl;
    std::cout << std::setw(90) << "insert <table name> <column1> ... <column n>"
              << "inserts new row in table <table name> with values <column1> ... <column n> in each column" << std::endl;
    std::cout << std::setw(90) << "innerjoin <table 1> <column n1> <table 2> <column n2>"
              << "inner joins table <table 1> and table <table 2> by columns <column 1> in <table 1> and <column 2> in <table 2>" << std::endl;
    std::cout << std::setw(90) << "rename <old name> <new name>"
              << "renames table <old name> with name <new name>" << std::endl;
    std::cout << std::setw(90) << "count <table name> <search column n> <search value>"
              << "finds count of rows in table <table name> that contain value <search value> in column <search column n>" << std::endl;
    std::cout << std::setw(90) << "aggregate <table name> <search column n> <search value> <target column n> <operation>"
              << "executes operation <operation> over every value in column <target column n> that contains value <search value> in column <search column n> in table <table name> " << std::endl;
}

void DatabaseHandler::save()
{
    std::ofstream os{
        file,
        std::ios::out};

    if (!os)
    {
        throw std::runtime_error("Couldn't save the file!");
    }

    database.serialize(os);
    os.close();
    isOpened = false;
    std::cout << "Successfully saved " << file << '\n';
    file = "";
    database = Database();
}

void DatabaseHandler::saveas(const std::string &filename)
{
    std::ofstream os{
        filename,
        std::ios::out};

    if (!os)
    {
        throw std::runtime_error("Couldn't save the file!");
    }

    database.serialize(os);
    os.close();
    isOpened = false;
    std::cout << "Successfully saved another " << filename << '\n';
    file = "";
    database = Database();
}

void DatabaseHandler::close() {
    isOpened = false;
    std::cout << "Successfully closed " << file << '\n';
    file = "";
    database = Database();
}