#include <fstream>
#include <exception>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <algorithm>
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "database_handler.hpp"

size_t DatabaseHandler::stringToInt(const std::string &str) const
{
    size_t result = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        result *= 10;
        result += str[i] - '0';
    }
    return result;
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
                throw;
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
            size_t column = stringToInt(parameters[0]);

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
            size_t searchColumn = stringToInt(parameters[1]);
            size_t targetColumn = stringToInt(parameters[3]);
            try
            {
                update(parameters[0], searchColumn, parameters[2], targetColumn, parameters[4]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "delete" && isOpened)
        {
            size_t column = stringToInt(parameters[1]);
            try
            {
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
                size_t column1 = stringToInt(parameters[1]);
                size_t column2 = stringToInt(parameters[3]);
                innerjoin(parameters[0], column1, parameters[2], column2);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command == "rename" && isOpened)
        {
            rename(parameters[0], parameters[1]);
        }
        else if (command == "count" && isOpened)
        {
            size_t column = stringToInt(parameters[1]);
            count(parameters[0], column, parameters[2]);
        }
        else if (command == "aggregate" && isOpened)
        {
            try
            {
                size_t searchColumn = stringToInt(parameters[1]);
                size_t targetColumn = stringToInt(parameters[3]);
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
        std::cout << "\nYou are currently in visualization mode! Available commands: \n";
        std::cout << std::setw(90) << std::left << "next"
                  << "Shows the next 10 rows of the table" << std::endl;
        std::cout << std::setw(90) << std::left << "previous"
                  << "Shows the previous 10 rows of the table" << std::endl;
        std::cout << std::setw(90) << std::left << "exit"
                  << "Exits the visualization mode" << std::endl;
        std::cout << '\n';

        size_t pages = table->getRows() / 10;
        if (table->getRows() % 10 > 0)
        {
            pages++;
        }

        size_t currentPage = 1;
        std::cout << "Current page: 1\n";

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

        std::string command;
        do
        {
            std::cout << "> ";
            std::getline(std::cin, command);
            if (command == "next")
            {
                if (pages > currentPage)
                {
                    currentPage++;
                    std::cout << "Current page: " << currentPage << "\n";

                    for (size_t i = 0; i < table->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*table)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage-1)*10; i < std::min(table->getRows(), currentPage * 10); i++)
                    {
                        for (size_t j = 0; j < table->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*table)[j])[i] << ' ';
                        }
                        std::cout << '\n';
                    }
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
                    std::cout << "Current page: " << currentPage << "\n";

                    for (size_t i = 0; i < table->getColumns(); i++)
                    {
                        std::cout << std::setw(20) << std::left << (*table)[i]->getName() << ' ';
                    }
                    std::cout << '\n';
                    for (size_t i = (currentPage-1)*10; i < currentPage * 10; i++)
                    {
                        for (size_t j = 0; j < table->getColumns(); j++)
                        {
                            std::cout << std::setw(20) << std::left << (*(*table)[j])[i] << ' ';
                        }
                        std::cout << '\n';
                    }
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
                std::cout << std::setw(90) << std::left << "next"
                          << "Shows the next 10 rows of the table" << std::endl;
                std::cout << std::setw(90) << std::left << "previous"
                          << "Shows the previous 10 rows of the table" << std::endl;
                std::cout << std::setw(90) << std::left << "exit"
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
        for (size_t i = 0; i < (*searchTable)[column - 1]->size(); i++)
        {
            if ((*(*searchTable)[column - 1])[i] == value)
            {
                for (size_t j = 0; j < searchTable->getColumns(); j++)
                {
                    std::cout << std::setw(20) << std::left << (*(*searchTable)[j])[i] << ' ';
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

void DatabaseHandler::update(const std::string &table, size_t searchColumn, const std::string &searchValue, size_t targetColumn, const std::string &targetValue)
{
    Table *searchTable = database.find(table);
    if (searchTable != nullptr)
    {
        for (size_t i = 0; i < (*searchTable)[searchColumn - 1]->size(); i++)
        {
            if ((*(*searchTable)[searchColumn - 1])[i] == searchValue)
            {
                (*(*searchTable)[targetColumn - 1])[i] = targetValue;
            }
        }
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
        std::vector<std::string> values(parameters);
        values.erase(std::begin(values));
        table->insertRow(values);
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
        std::cout << "Successfully joined tables " << firstTable->getName() << " and " << secondTable->getName() << " into " << result.getName() << "!\n";
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