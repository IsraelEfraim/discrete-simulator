#include "QueuePolicy.hpp"

#include <iostream>
#include <limits>

auto simulation::QueuePolicy::isFull(size_t queueSize) const -> bool {
    return !(queueSize < this->max);
}

auto simulation::limitedQueue(size_t size) -> QueuePolicy {
    return { size };
}

auto simulation::unlimitedQueue() -> QueuePolicy {
    return { std::numeric_limits<size_t>::max() };
}
