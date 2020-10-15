#ifndef EVENT_HPP
#define EVENT_HPP

#include "Types.hpp"

namespace simulation {
    enum class EventType { Start, Arrival, Departure, End };

    struct Event {
        EventType type;
        TimeUnit time;
        size_t entityRef;
    };

    struct EventComparator {
        auto operator()(Event const& l, Event const& r) -> bool {
            return l.time < r.time;
        }
    };
}

#endif // EVENT_HPP
