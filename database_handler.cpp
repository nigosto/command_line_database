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
            try
            {
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
            database.printTableNames();
        }
        else if (command == "describe" && isOpened)
        {
            describe(parameters[0]);
        }
        else if (command == "print" && isOpened)
        {
            print(parameters[0]);
        }
        else if (command == "export" && isOpened)
        {
            try
            {
                exportTable(parameters[0], parameters[1]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "select" && isOpened)
        {
            size_t column = 0;
            for (size_t i = 0; i < parameters[0].size(); i++)
            {
                column *= 10;
                column += parameters[0][i] - '0';
            }

            try
            {
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
                addcolumn(parameters[0], parameters[1], parameters[2]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
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
            else
            {
                std::cout << "Exiting program...\n";
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

void DatabaseHandler::close()
{
    isOpened = false;
    std::cout << "Successfully closed " << file << '\n';
    file = "";
    database = Database();
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
        for (size_t i = 0; i < table->getColumns(); i++)
        {
            std::cout << std::setw(30) << std::left << (*table)[i]->getName() << ' ';
        }
        std::cout << '\n';
        for (size_t i = 0; i < table->getRows(); i++)
        {
            for (size_t j = 0; j < table->getColumns(); j++)
            {
                std::cout << std::setw(30) << std::left << (*(*table)[j])[i] << ' ';
            }
            std::cout << '\n';
        }
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
        for (size_t i = 0; i < (*searchTable)[column - 1]->size(); i++)
        {
            if ((*(*searchTable)[column - 1])[i] == value)
            {
                for (size_t j = 0; j < searchTable->getColumns(); j++)
                {
                    std::cout << std::setw(30) << std::left << (*(*searchTable)[j])[i] << ' ';
                }
                std::cout << '\n';
            }
        }
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
        std::cout << "Column " << column << " added successfully to table " << table << '\n';
    }
    else
    {
        std::cout << "Couldn't find table with specified name!\n";
    }
}
