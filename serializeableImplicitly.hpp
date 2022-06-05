#ifndef __ISERIALIZEABLE_IMPLICITLY_HPP
#define __ISERIALIZEABLE_IMPLICITLY_HPP

#include <iostream>

class ISerializeableImplicitly {
public:
    virtual void serialize() const = 0;
    virtual void deserialize() = 0;
};

#endif