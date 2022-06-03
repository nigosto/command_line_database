#include <iostream>
#include "intColumn.hpp"
#include "doubleColumn.hpp"
#include "stringColumn.hpp"

int main() {
    IntColumn a(5);
    a.insertCell("152");
    a[0] = "15";
    std::cout<<a.type()<<'\n';
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout<<a[i]<<'\n';
    }
    std::cout<<'\n';
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout<<a.valueAsNumber(i)<<'\n';
    }
    return 0;
}