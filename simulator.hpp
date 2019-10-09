#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "distribution.hpp"

#include <optional>
#include <vector>
#include <queue>

namespace simulation {

    using TimeUnit = double;

    struct Entity {
        size_t id;
        TimeUnit arrival;
        TimeUnit processStart;
        TimeUnit processEnd;
        TimeUnit departure;
    };

    enum class EventType { Start, Arrival, Departure, End };

    struct Event {
        EventType type;
        TimeUnit time;
        std::optional<size_t> entityRef;
    };

    struct EventComparator {
        auto operator()(Event const& l, Event const& r) -> bool {
            return l.time < r.time;
        }
    };

    struct Resource {
        bool working;
        TimeUnit endJob;
    };

    struct StateStatistics {
        /* entity */
        TimeUnit entityWaitingTime;
        TimeUnit entityServiceTime;
        TimeUnit entitySystemTime;

        /* resource */
        TimeUnit resourceIdleTime;

        /* system */
        size_t queueCount;
        size_t entityCount;
    };

    struct State {
        TimeUnit clock;
        Resource resource;
        bool over;

        std::queue<Entity> queue;
        std::vector<Event> events;

        std::vector<StateStatistics> statistics;
    };

    struct SimulationController {
        using RandomGenerator = distribution::types::RandomGenerator;

        RandomGenerator tecGenerator;
        RandomGenerator tsGenerator;
    };

    auto cleanState() -> State {
        return { 0.0, Resource{ false, 0.0 }, false, std::queue<Entity>{},
                       std::vector<Event>{},  std::vector<StateStatistics>{} };
    }

    auto advanceTime(State &state) -> Event {
        /* Take iminent event and advance clock */
        Event iminent = state.events.front();
        state.clock = iminent.time;

        /* Remove iminent event from the vector */
        state.events.erase(state.events.begin());

        return iminent;
    }

    auto consumeEvent(Event const& event, State &state, SimulationController const& controller) -> void {
        switch (event.type) {
            case EventType::Start:
                break;
            case EventType::Arrival:

                break;
            case EventType::Departure:
                break;
            case EventType::End:
                break;
        }
    }

    auto simulate(State const& state, SimulationController const& controller) -> State {
        State newState = state;

        Event iminent = advanceTime(newState);

        return newState;
    }
}



#endif // SIMULATOR_HPP
