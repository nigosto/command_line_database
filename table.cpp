#include <fstream>
#include "table.hpp"
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"

Table::Table(std::string &&_name, std::string &&_filename)
: name(std::move(_name)), filename(std::move(_filename)), rows_count(0), columns_count(0) {}

Table::Table(std::string &_name, std::string &_filename)
    : name(_name), filename(_filename), rows_count(0), columns_count(0) {}

Table::Table(const Table &other)
    : name(other.name), filename(other.filename), rows_count(other.rows_count), columns_count(other.columns_count), columns(other.columns)
{
    for (size_t i = 0; i < other.columns_count; i++)
    {
        columns[i] = other.columns[i]->clone();
    }
}

Table::Table(Table &&other)
{
    // TODO
}

Table::~Table()
{
    for (size_t i = 0; i < columns_count; i++)
    {
        delete columns[i];
    }
}

Table &Table::operator=(const Table &other)
{
    Table copy(other);
    swap(copy);
    return *this;
}

Table &Table::operator=(Table &&other)
{
    // TODO
}

void Table::swap(Table &other)
{
    using std::swap;
    std::swap(name, other.name);
    std::swap(filename, other.filename);
    std::swap(columns_count, other.columns_count);
    std::swap(rows_count, other.rows_count);
    std::swap(columns, other.columns);
}

void Table::addColumn(const std::string &_name, const std::string &_type)
{
    if (_type == "integer")
    {
        columns.push_back(new IntColumn(_name, rows_count));
    }
    else if (_type == "double")
    {
        columns.push_back(new DoubleColumn(_name, rows_count));
    }
    else if (_type == "string")
    {
        columns.push_back(new StringColumn(_name, rows_count));
    }
    columns_count++;
}

std::string *Table::find(size_t columnIndex, const std::string &value)
{
    // TODO
}

void Table::insertRow(const std::vector<std::string> values)
{
    rows_count++;
    for (size_t i = 0; i < values.size(); i++)
    {
        columns[i]->push_back(values[i]);
    }
    for (size_t i = values.size(); i < columns_count; i++)
    {
        columns[i]->push_back("NULL");
    }
}

void Table::rename(const std::string &_name)
{
    name = _name;
}

void Table::serialize() const
{
    std::ofstream os {
        filename,
        std::ios::out
    };

    os << name << '\n' << filename << '\n'<< columns_count << '\n'<< rows_count << '\n';
    for (size_t i = 0; i < columns_count; i++)
    {
        columns[i]->serialize(os);
    }
    os.close();
}

void Table::deserialize()
{
    // TODO
}
