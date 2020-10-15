#ifndef QUEUEPOLICY_HPP
#define QUEUEPOLICY_HPP

namespace simulation {
    struct QueuePolicy {
        size_t max;
        auto isFull(size_t queueSize) const -> bool;
    };

    auto limitedQueue(size_t size) -> QueuePolicy;
    auto unlimitedQueue() -> QueuePolicy;
}

#endif // QUEUEPOLICY_HPP
