#include <iostream>
#include <fstream>
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"
#include "table.hpp"
#include "database.hpp"

int main()
{
    // IntColumn a(5);
    // a.push_back("+152");
    // a[0] = "-15";
    // std::cout << a.type() << '\n';
    // for (size_t i = 0; i < a.size(); i++)
    // {
    //     std::cout << a[i] << '\n';
    // }
    // std::cout << '\n';
    // for (size_t i = 0; i < a.size(); i++)
    // {
    //     std::cout << a.valueAsNumber(i) << '\n';
    // }

    // std::ofstream os{
    //     "test.txt",
    //     std::ios::out};

    // a.serialize(os);
    // os.close();
    // std::cout << '\n';
    // std::ifstream is{
    //     "test.txt",
    //     std::ios::in};

    // IntColumn b;

    // std::string type;
    // is >> type;
    // if (type == "integer")
    // {
    //     b.deserialize(is);
    // }
    // b.serialize();

    // Table t("table", "table.txt");
    // t.addColumn("quantity", "integer");
    // t.addColumn("rating", "integer");
    // std::vector<std::string> v1 {"12", "+17"};
    // std::vector<std::string> v2 {"5"};
    // t.insertRow(v1);
    // t.insertRow(v2);
    // t.addColumn("diopter", "double");

    // t.print();
    // t.serialize();

    std::ifstream is{
        "database.txt",
        std::ios::in};

    Database db;
    db.deserialize(is);
    is.close();
    try
    {
        db.import("table.txt");

        std::ofstream os{
            "database.txt",
            std::ios::out};
        db.serialize(os);
        os.close();
        Table *table = db.find("table");
        if (table != nullptr)
        {
            table->print();
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}