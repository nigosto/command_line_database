#include <fstream>
#include <algorithm>
#include "table.hpp"
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"

Table::Table(const std::string &_filename, const std::string &_name)
    : name(_name), filename(_filename), rows_count(0), columns_count(0) {}

Table::Table(const Table &other)
    : name(other.name), filename(other.filename), rows_count(other.rows_count), columns_count(other.columns_count), columns(other.columns)
{
    for (size_t i = 0; i < other.columns_count; i++)
    {
        columns[i] = other.columns[i]->clone();
    }
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
    columns.push_back(Column::createColumn(_type, _name, rows_count));
    columns_count++;
}

void Table::insertRow(const std::vector<std::string> &values)
{
    for (size_t i = 0; i < getColumns(); i++)
    {
        if (values.size() >= i + 1)
        {
            columns[i]->push_back(values[i]);
        }
        else
        {
            columns[i]->push_back("NULL");
        }
    }
    rows_count++;
}

Column *Table::operator[](size_t index)
{
    if (index < 0 || index > columns.size() - 1)
    {
        throw std::runtime_error("Invalid index!");
    }
    return columns[index];
}

const Column *Table::operator[](size_t index) const
{
    if (index < 0 || index > columns.size() - 1)
    {
        throw std::runtime_error("Invalid index!");
    }
    return columns[index];
}

void Table::rename(const std::string &_name)
{
    name = _name;
}

void Table::serialize(bool recovery) const
{
    std::ofstream os;
    if (recovery)
    {
        os = std::ofstream{
            "recovery-" + filename,
            std::ios::out};
    }
    else
    {
        os = std::ofstream{
            filename,
            std::ios::out};
    }

    os << name << '\n'
       << columns_count << '\n'
       << rows_count << '\n';
    for (size_t i = 0; i < columns_count; i++)
    {
        columns[i]->serialize(os);
    }
    os.close();
}

void Table::deserialize(bool recovery)
{
    std::ifstream is;
    if (recovery)
    {
        is = std::ifstream{
            "recovery-" + filename,
            std::ios::in};
    }
    else
    {
        is = std::ifstream{
            filename,
            std::ios::in};
    }

    std::string _name;
    std::getline(is, _name);
    name = _name;
    size_t _columns_count;
    size_t _rows_count;
    is >> _columns_count >> _rows_count;
    columns_count = _columns_count;
    rows_count = _rows_count;
    is.ignore();
    for (size_t i = 0; i < _columns_count; i++)
    {
        std::string type;
        std::getline(is, type);
        columns.push_back(Column::createColumn(type));
        columns[i]->deserialize(is);
    }
    is.close();
}

void Table::remove(size_t rowIndex)
{
    size_t count = getColumns();
    for (size_t i = 0; i < count; i++)
    {
        columns[i]->remove(rowIndex);
    }
    rows_count--;
}

Table innerJoin(const Table &first, size_t firstColumn, const Table &second, size_t secondColumn)
{

    Table result(first.getName() + '-' + second.getName() + ".txt", first.getName() + '-' + second.getName());
    for (size_t i = 0; i < first.getColumns(); i++)
    {
        if (i == firstColumn)
        {
            result.addColumn(first.getName() + '-' + first.columns[i]->getName(), first.columns[i]->type());
        }
        else
        {
            result.addColumn(first.columns[i]->getName(), first.columns[i]->type());
        }
    }
    for (size_t i = 0; i < second.getColumns(); i++)
    {
        if (i == secondColumn)
        {
            result.addColumn(second.getName() + '-' + second.columns[i]->getName(), second.columns[i]->type());
        }
        else
        {
            result.addColumn(second.columns[i]->getName(), second.columns[i]->type());
        }
    }

    for (size_t i = 0; i < std::min(first.getRows(), second.getRows()); i++)
    {
        for (size_t j = 0; j < std::max(first.getRows(), second.getRows()); j++)
        {
            if ((*first.columns[firstColumn])[i] == (*second.columns[secondColumn])[j])
            {
                std::vector<std::string> v;
                for (size_t k = 0; k < first.getColumns(); k++)
                {
                    v.push_back((*first.columns[k])[i]);
                }
                for (size_t k = 0; k < second.getColumns(); k++)
                {
                    v.push_back((*second.columns[k])[j]);
                }

                result.insertRow(v);
            }
        }
    }

    return result;
}