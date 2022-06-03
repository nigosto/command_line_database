#ifndef __ISERIALIZEABLE_HPP
#define __ISERIALIZEABLE_HPP

#include <iostream>

class ISerializeable {
public:
    virtual std::ostream& serialize(std::ostream& os) const = 0;
    virtual std::istream& deserialize(std::istream& is) = 0;
};

#endif