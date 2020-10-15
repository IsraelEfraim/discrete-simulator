#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Types.hpp"

namespace simulation {
    struct Entity {
        size_t id;
        TimeUnit arrival;
        TimeUnit processStart;
        TimeUnit processEnd;
        TimeUnit departure;
    };
}

#endif // ENTITY_HPP
