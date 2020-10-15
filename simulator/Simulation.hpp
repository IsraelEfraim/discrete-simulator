#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "Distribution.hpp"
#include "Types.hpp"
#include "Entity.hpp"
#include "Resource.hpp"
#include "QueuePolicy.hpp"
#include "Event.hpp"
#include "State.hpp"

#include <algorithm>

namespace simulation {
    auto cleanState(TimeUnit start = 0.0, TimeUnit end = 0.0) -> State;

    auto advanceTime(State &state) -> Event;

    auto consumeStart(Event const& event, State &state, SimulationController const& controller) -> void;
    auto consumeArrival(Event const& event, State &state, SimulationController const& controller) -> void;
    auto consumeDeparture(Event const&, State &state, SimulationController const&) -> void;
    auto consumeEnd(Event const& event, State &state, SimulationController const& controller) -> void;
    auto consumeEvent(Event const& event, State &state, SimulationController const& controller) -> void;

    auto updateResource(State &state, SimulationController const& controller) -> void;

    auto calculate(State const& state) -> SimulationStatistics;

    auto simulate(State const& state, SimulationController const& controller) -> State;
}



#endif // SIMULATION_HPP
