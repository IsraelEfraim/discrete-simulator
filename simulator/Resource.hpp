#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Types.hpp"
#include "Entity.hpp"

namespace simulation {
    struct Resource {
        bool working;
        TimeUnit workingUntil;
        TimeUnit idle;
        Entity* currentEntity;
    };
}

#endif // RESOURCE_HPP
