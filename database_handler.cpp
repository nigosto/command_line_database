#include <fstream>
#include <exception>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cstdio>
#include "stringManipulator.hpp"
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "database_handler.hpp"

void DatabaseHandler::saveTo(const std::string &filename)
{
    std::ofstream os{
        filename,
        std::ios::out};

    if (!os)
    {
        throw std::runtime_error("Couldn't save file!");
    }

    if (filename == "recovery.txt")
    {
        database.serializeWithRecovery(os);
    }
    else
    {
        database.serialize(os);
    }
    os.close();
}

DatabaseHandler &DatabaseHandler::getInstance()
{
    static DatabaseHandler instance;
    return instance;
}

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
        command = StringManipulator::trimRight(StringManipulator::trimLeft(command));

        size_t pos = command.find(space_delimiter);
        while (pos != std::string::npos)
        {
            parameters.push_back(command.substr(0, pos));
            command.erase(0, pos + space_delimiter.length());
            command = StringManipulator::trimLeft(command);
            pos = command.find(space_delimiter);
        }
        parameters.push_back(command);
        command = parameters[0];
        StringManipulator::toLowerCase(command);
        parameters.erase(std::begin(parameters));

        if (command == "open" && !isOpened)
        {
            try
            {
                if (parameters.size() != 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
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
            try
            {
                if (parameters.size() != 0)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                help();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "save" && isOpened)
        {
            try
            {
                if (parameters.size() != 0)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                save();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "saveas" && isOpened)
        {
            try
            {
                if (parameters.size() != 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                saveas(parameters[0]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "close" && isOpened)
        {
            if (parameters.size() != 0)
            {
                throw std::runtime_error("Invalid number of parameters!");
            }
            close();
        }
        else if (command == "exit" && isOpened)
        {
            std::cout << "You have an open file with unsaved changes, please select close or save first.\n";
            command = "";
        }
        else if (command == "import" && isOpened)
        {
            try
            {
                if (parameters.size() != 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                database.import(parameters[0]);
                std::cout << "Table successfully imported to database!\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "showtables" && isOpened)
        {
            try
            {
                if (parameters.size() != 0)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                showtables();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "describe" && isOpened)
        {
            try
            {
                if (parameters.size() != 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                describe(parameters[0]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "print" && isOpened)
        {
            try
            {
                if (parameters.size() != 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                print(parameters[0]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "export" && isOpened)
        {
            try
            {
                if (parameters.size() != 2)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                exportTable(parameters[0], parameters[1]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "select" && isOpened)
        {
            try
            {
                if (parameters.size() != 3)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t column = StringManipulator::stringToInt(parameters[0]);

                select(column, parameters[1], parameters[2]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "addcolumn" && isOpened)
        {
            try
            {
                if (parameters.size() != 3)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                addcolumn(parameters[0], parameters[1], parameters[2]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "update" && isOpened)
        {
            try
            {
                if (parameters.size() != 5)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t searchColumn = StringManipulator::stringToInt(parameters[1]);
                size_t targetColumn = StringManipulator::stringToInt(parameters[3]);
                update(parameters[0], searchColumn, parameters[2], targetColumn, parameters[4]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "delete" && isOpened)
        {
            try
            {
                if (parameters.size() != 3)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t column = StringManipulator::stringToInt(parameters[1]);
                deleteRows(parameters[0], column, parameters[2]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "insert" && isOpened)
        {
            try
            {
                if (parameters.size() < 1)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                insert(parameters);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "innerjoin" && isOpened)
        {
            try
            {
                if (parameters.size() != 4)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t column1 = StringManipulator::stringToInt(parameters[1]);
                size_t column2 = StringManipulator::stringToInt(parameters[3]);
                innerjoin(parameters[0], column1, parameters[2], column2);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "rename" && isOpened)
        {
            try
            {
                if (parameters.size() != 2)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                rename(parameters[0], parameters[1]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "count" && isOpened)
        {
            try
            {
                if (parameters.size() != 3)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t column = StringManipulator::stringToInt(parameters[1]);
                count(parameters[0], column, parameters[2]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "aggregate" && isOpened)
        {
            try
            {
                if (parameters.size() != 5)
                {
                    throw std::runtime_error("Invalid number of parameters!");
                }
                size_t searchColumn = StringManipulator::stringToInt(parameters[1]);
                size_t targetColumn = StringManipulator::stringToInt(parameters[3]);
                aggregate(parameters[0], searchColumn, parameters[2], targetColumn, parameters[4]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
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
            else
            {
                std::cout << "Exiting program...\n";
            }
        }
    } while (command != "exit");
}

void DatabaseHandler::open(const std::string &filename)
{
    std::ifstream recovery{
        "recovery.txt",
        std::ios::in};

    if (recovery)
    {
        std::cout << "Looks like the program didn't close properly last time. There is a recovery file, containing all the changes made before. \nWould you like to open the recovery file instead? (y/n): ";
        char c;
        std::cin >> c;
        std::cin.ignore();
        if (c == 'y' || c == 'Y')
        {
            database.deserialize(recovery);
            recovery.close();
            isOpened = true;
            file = filename;
            std::cout << "Successfully opened recovery file. New changes will be saved in: " << filename << '\n';
            return;
        }
    }

    std::ifstream is{
        filename,
        std::ios::in};

    database.deserialize(is);
    is.close();
    isOpened = true;
    file = filename;
    std::cout << "Successfully opened " << filename << '\n';

    for (size_t i = 0; i < database.size(); i++)
    {
        database[i].serialize(true);
    }
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
    saveTo(file);
    std::remove("recovery.txt");
    for (size_t i = 0; i < database.size(); i++)
    {
        std::string s = ("recovery-" + database[i].getFilename());
        std::remove(s.c_str());
    }
    isOpened = false;
    std::cout << "Successfully saved " << file << '\n';
    file = "";
    database = Database();
}

void DatabaseHandler::saveas(const std::string &filename)
{
    saveTo(filename);
    std::remove("recovery.txt");
    for (size_t i = 0; i < database.size(); i++)
    {
        std::string s = ("recovery-" + database[i].getFilename());
        std::remove(s.c_str());
    }

    isOpened = false;
    std::cout << "Successfully saved another " << filename << '\n';
    file = "";
    database = Database();
}

void DatabaseHandler::close()
{
    std::remove("recovery.txt");
    for (size_t i = 0; i < database.size(); i++)
    {
        std::string s = ("recovery-" + database[i].getFilename());
        std::remove(s.c_str());
    }

    isOpened = false;
    std::cout << "Successfully closed " << file << '\n';
    file = "";
    database = Database();
}

void DatabaseHandler::showtables() const
{
    std::cout << "List of all tables: \n";
    for (size_t i = 0; i < database.size(); i++)
    {
        std::cout << i + 1 << ") " << database[i].getName() << '\n';
    }
}

void DatabaseHandler::describe(const std::string &name)
{
    Table *table = database.find(name);
    if (table != nullptr)
    {
        std::cout << "Types of table " << name << ":\n";
        for (size_t i = 0; i < table->getColumns(); i++)
        {
            std::cout << i + 1 << ") " << (*table)[i]->type() << '\n';
        }
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::print(const std::string &name)
{
    Table *table = database.find(name);
    if (table != nullptr)
    {
        std::cout << "\nYou are currently in visualization mode! Available commands: \n";
        std::cout << std::setw(20) << std::left << "next"
                  << "Shows the next 10 rows of the table" << std::endl;
        std::cout << std::setw(20) << std::left << "previous"
                  << "Shows the previous 10 rows of the table" << std::endl;
        std::cout << std::setw(20) << std::left << "exit"
                  << "Exits the visualization mode" << std::endl;

        size_t pages = table->getRows() / 10;
        if (table->getRows() % 10 > 0)
        {
            pages++;
        }

        size_t currentPage = 1;
        std::cout << "\nCurrent page: 1\n";

        for (size_t i = 0; i < table->getColumns(); i++)
        {
            std::cout << std::setw(20) << std::left << (*table)[i]->getName() << ' ';
        }
        std::cout << '\n';
        for (size_t i = 0; i < std::min(table->getRows(), currentPage * 10); i++)
        {
            for (size_t j = 0; j < table->getColumns(); j++)
            {
                std::cout << std::setw(20) << std::left << (*(*table)[j])[i] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        std::string command;
        do
        {
            std::cout << "> ";
            std::getline(std::cin, command);
            command = StringManipulator::trimRight(StringManipulator::trimLeft(command));
            StringManipulator::toLowerCase(command);

            if (command == "next")
            {
                if (pages > currentPage)
                {
                    currentPage++;
                    std::cout << "\nCurrent page: " << currentPage << "\n";

                    for (size_t i = 0; i < table->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*table)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage - 1) * 10; i < std::min(table->getRows(), currentPage * 10); i++)
                    {
                        for (size_t j = 0; j < table->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*table)[j])[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "No more pages!\n";
                }
            }
            else if (command == "previous")
            {
                if (currentPage > 1)
                {
                    currentPage--;
                    std::cout << "\nCurrent page: " << currentPage << "\n";

                    for (size_t i = 0; i < table->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*table)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage - 1) * 10; i < currentPage * 10; i++)
                    {
                        for (size_t j = 0; j < table->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*table)[j])[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "There are no previous pages!\n";
                }
            }
            else if (command == "exit")
            {
                std::cout << "Exiting visualization mode...\n";
            }
            else
            {
                std::cout << "Invalid command! Available commands: \n";
                std::cout << std::setw(20) << std::left << "next"
                          << "Shows the next 10 rows of the table" << std::endl;
                std::cout << std::setw(20) << std::left << "previous"
                          << "Shows the previous 10 rows of the table" << std::endl;
                std::cout << std::setw(20) << std::left << "exit"
                          << "Exits the visualization mode" << std::endl;
            }
        } while (command != "exit");
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::exportTable(const std::string &name, const std::string &filename)
{
    Table *searchTable = database.find(name);
    if (searchTable != nullptr)
    {
        Table targetTable(*searchTable);
        targetTable.changeFilename(filename);
        targetTable.serialize();
        std::cout << "Table exported successfully!\n";
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::select(size_t column, const std::string &value, const std::string &table)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        std::vector<size_t> indexes;
        for (size_t i = 0; i < (*searchTable)[column - 1]->size(); i++)
        {
            if ((*(*searchTable)[column - 1])[i] == value)
            {
                indexes.push_back(i);
            }
        }

        std::cout << "\nYou are currently in visualization mode! Available commands: \n";
        std::cout << std::setw(20) << std::left << "next"
                  << "Shows the next 10 rows of the table" << std::endl;
        std::cout << std::setw(20) << std::left << "previous"
                  << "Shows the previous 10 rows of the table" << std::endl;
        std::cout << std::setw(20) << std::left << "exit"
                  << "Exits the visualization mode" << std::endl;

        size_t pages = indexes.size() / 10;
        pages += (indexes.size() % 10 != 0);
        size_t currentPage = 1;

        std::cout << "\nCurrent page: 1\n";

        for (size_t i = 0; i < searchTable->getColumns(); i++)
        {
            std::cout << std::setw(20) << std::left << (*searchTable)[i]->getName() << ' ';
        }
        std::cout << '\n';
        for (size_t i = 0; i < std::min(indexes.size(), currentPage * 10); i++)
        {
            for (size_t j = 0; j < searchTable->getColumns(); j++)
            {
                std::cout << std::setw(20) << std::left << (*(*searchTable)[j])[indexes[i]] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        std::string command;
        do
        {
            std::cout << "> ";
            std::getline(std::cin, command);
            command = StringManipulator::trimRight(StringManipulator::trimLeft(command));
            StringManipulator::toLowerCase(command);

            if (command == "next")
            {
                if (pages > currentPage)
                {
                    currentPage++;
                    std::cout << "\nCurrent page: " << currentPage << "\n";

                    for (size_t i = 0; i < searchTable->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*searchTable)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage - 1) * 10; i < std::min(indexes.size(), currentPage * 10); i++)
                    {
                        for (size_t j = 0; j < searchTable->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*searchTable)[j])[indexes[i]] << ' ';
                        }
                        std::cout << '\n';
                    }
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "No more pages!\n";
                }
            }
            else if (command == "previous")
            {
                if (currentPage > 1)
                {
                    currentPage--;
                    std::cout << "\nCurrent page: " << currentPage << "\n";

                    for (size_t i = 0; i < searchTable->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*searchTable)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage - 1) * 10; i < currentPage * 10; i++)
                    {
                        for (size_t j = 0; j < searchTable->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*searchTable)[j])[indexes[i]] << ' ';
                        }
                        std::cout << '\n';
                    }
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "There are no previous pages!\n";
                }
            }
            else if (command == "exit")
            {
                std::cout << "Exiting visualization mode...\n";
            }
            else
            {
                std::cout << "Invalid command! Available commands: \n";
                std::cout << std::setw(20) << std::left << "next"
                          << "Shows the next 10 rows of the table" << std::endl;
                std::cout << std::setw(20) << std::left << "previous"
                          << "Shows the previous 10 rows of the table" << std::endl;
                std::cout << std::setw(20) << std::left << "exit"
                          << "Exits the visualization mode" << std::endl;
            }
        } while (command != "exit");
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::addcolumn(const std::string &table, const std::string &column, const std::string &type)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        searchTable->addColumn(column, type);
        searchTable->serialize(true);
        saveTo("recovery.txt");
        std::cout << "Column " << column << " added successfully to table " << table << '\n';
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::update(const std::string &table, size_t searchColumn, const std::string &searchValue, size_t targetColumn, const std::string &targetValue)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
        {
            if ((*(*searchTable)[searchColumn - 1])[i] == searchValue)
            {
                (*searchTable)[targetColumn - 1]->updateValue(i, targetValue);
            }
        }
        searchTable->serialize(true);
        saveTo("recovery.txt");
        std::cout << "Table " << table << " successfully updated!\n";
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::deleteRows(const std::string &table, size_t column, const std::string &value)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        for (size_t i = 0; i < (*searchTable)[column - 1]->size(); i++)
        {
            if ((*(*searchTable)[column - 1])[i] == value)
            {
                searchTable->remove(i);
                i--;
            }
        }

        searchTable->serialize(true);
        saveTo("recovery.txt");
        std::cout << "Rows deleted successfully!\n";
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::insert(const std::vector<std::string> &parameters)
{
    Table *table = database.find(parameters[0]);
    if (table != nullptr)
    {
        if (parameters.size() - 1 > table->getColumns())
        {
            throw std::runtime_error("Invalid number of parameters!");
        }
        std::vector<std::string> values(parameters);
        values.erase(std::begin(values));
        table->insertRow(values);

        table->serialize(true);
        saveTo("recovery.txt");
        std::cout << "Row inserted successfully!\n";
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::innerjoin(const std::string &table1, size_t column1, const std::string &table2, size_t column2)
{
    Table *firstTable = database.find(table1);
    Table *secondTable = database.find(table2);
    if (firstTable != nullptr && secondTable != nullptr)
    {
        Table result = innerJoin(*firstTable, column1 - 1, *secondTable, column2 - 1);
        result.serialize();
        database.import(result.getFilename());

        std::string firstName = firstTable->getName();
        std::string secondName = secondTable->getName();

        result.serialize(true);
        saveTo("recovery.txt");
        std::cout << "Successfully joined tables " << firstName << " and " << secondName << " into " << result.getName() << "!\n";
    }
    else
    {
        std::cout << "Couldn't find one of the tables!\n";
    }
}

void DatabaseHandler::rename(const std::string &oldName, const std::string &newName)
{
    Table *table = database.find(oldName);
    if (table != nullptr)
    {
        if (database.find(newName) != nullptr)
        {
            std::cout << "A table with this name already exists!\n";
        }
        else
        {
            table->rename(newName);

            table->serialize(true);
            saveTo("recovery.txt");
            std::cout << "Table "
                      << "\"" << oldName << "\" "
                      << "renamed to "
                      << "\"" << newName << "\"!\n";
        }
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::count(const std::string &table, size_t column, const std::string &value)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        size_t counter = 0;
        for (size_t i = 0; i < (*searchTable)[column - 1]->size(); i++)
        {
            if ((*(*searchTable)[column - 1])[i] == value)
            {
                counter++;
            }
        }
        std::cout << counter << " rows in column " << (*searchTable)[column - 1]->getName() << " contain value " << value << "!\n";
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}

void DatabaseHandler::aggregate(const std::string &table, size_t searchColumn, const std::string &searchValue, size_t targetColumn, const std::string &operation)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        if ((*searchTable)[targetColumn - 1]->type() == "string")
        {
            std::cout << "Target column is not of numeric type!\n";
        }
        else
        {
            if (operation == "sum")
            {
                double sum = 0;

                for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
                {
                    if ((*(*searchTable)[searchColumn - 1])[i] == searchValue)
                    {

                        if ((*searchTable)[targetColumn - 1]->type() == "integer")
                        {
                            IntColumn *col = dynamic_cast<IntColumn *>((*searchTable)[targetColumn - 1]);
                            sum += col->valueAsNumber(i);
                        }
                        else if ((*searchTable)[targetColumn - 1]->type() == "double")
                        {
                            DoubleColumn *col = dynamic_cast<DoubleColumn *>((*searchTable)[targetColumn - 1]);
                            sum += col->valueAsNumber(i);
                        }
                    }
                }

                std::cout << "The sum is: " << sum << '\n';
            }
            else if (operation == "product")
            {
                double product = 1;

                for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
                {
                    if ((*(*searchTable)[searchColumn - 1])[i] == searchValue && (*(*searchTable)[targetColumn - 1])[i] != "NULL")
                    {

                        if ((*searchTable)[targetColumn - 1]->type() == "integer")
                        {
                            IntColumn *col = dynamic_cast<IntColumn *>((*searchTable)[targetColumn - 1]);
                            product *= col->valueAsNumber(i);
                        }
                        else if ((*searchTable)[targetColumn - 1]->type() == "double")
                        {
                            DoubleColumn *col = dynamic_cast<DoubleColumn *>((*searchTable)[targetColumn - 1]);
                            product *= col->valueAsNumber(i);
                        }
                    }
                }
                std::cout << "The product is: " << product << '\n';
            }
            else if (operation == "maximum")
            {
                double maximum = std::numeric_limits<double>::lowest();

                for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
                {
                    if ((*(*searchTable)[searchColumn - 1])[i] == searchValue && (*(*searchTable)[targetColumn - 1])[i] != "NULL")
                    {

                        if ((*searchTable)[targetColumn - 1]->type() == "integer")
                        {
                            IntColumn *col = dynamic_cast<IntColumn *>((*searchTable)[targetColumn - 1]);
                            if (col->valueAsNumber(i) > maximum)
                            {
                                maximum = col->valueAsNumber(i);
                            }
                        }
                        else if ((*searchTable)[targetColumn - 1]->type() == "double")
                        {
                            DoubleColumn *col = dynamic_cast<DoubleColumn *>((*searchTable)[targetColumn - 1]);
                            if (col->valueAsNumber(i) > maximum)
                            {
                                maximum = col->valueAsNumber(i);
                            }
                        }
                    }
                }
                std::cout << "The maximum value is: " << maximum << '\n';
            }
            else if (operation == "minimum")
            {
                double minimum = std::numeric_limits<double>::max();

                for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
                {
                    if ((*(*searchTable)[searchColumn - 1])[i] == searchValue && (*(*searchTable)[targetColumn - 1])[i] != "NULL")
                    {

                        if ((*searchTable)[targetColumn - 1]->type() == "integer")
                        {
                            IntColumn *col = dynamic_cast<IntColumn *>((*searchTable)[targetColumn - 1]);
                            if (col->valueAsNumber(i) < minimum)
                            {
                                minimum = col->valueAsNumber(i);
                            }
                        }
                        else if ((*searchTable)[targetColumn - 1]->type() == "double")
                        {
                            DoubleColumn *col = dynamic_cast<DoubleColumn *>((*searchTable)[targetColumn - 1]);
                            if (col->valueAsNumber(i) < minimum)
                            {
                                minimum = col->valueAsNumber(i);
                            }
                        }
                    }
                }
                std::cout << "The minimum value is: " << minimum << '\n';
            }
            else
            {
                std::cout << "Unrecognized operation!\n";
            }
        }
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}