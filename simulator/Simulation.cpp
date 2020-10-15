#include "Simulation.hpp"

#include <iostream>


auto simulation::cleanState(TimeUnit start, TimeUnit end) -> State {
    State state { 0.0, Resource{ false, 0.0, 0.0, nullptr }, 0, false, std::queue<Entity>{},
                std::vector<Event>{},  std::vector<StateStatistics>{},
                std::vector<SimulationStatistics>{}, Event{} };

    state.events.push_back({ EventType::Start, start, 0 });
    state.events.push_back({ EventType::End, end, 0 });

    state.iminent = state.events.front();

    return state;
}

auto simulation::advanceTime(State &state) -> Event {
    /* Take iminent event and advance clock */
    Event iminent = state.events.front();
    state.iminent = iminent;
    state.clock = iminent.time;

    /* Remove iminent event from the vector */
    state.events.erase(state.events.begin());

    return iminent;
}

auto simulation::consumeStart(Event const&, State &state, SimulationController const& controller) -> void {
    auto tec = controller.tecGenerator();
    auto arrival = state.clock + tec;
    std::cout << state.clock << " " << tec << std::endl;
    state.events.push_back({ EventType::Arrival, arrival, ++state.entityIndexer });
}

auto simulation::consumeArrival(Event const& event, State &state, SimulationController const& controller) -> void {
    auto idle = state.resource.working ? 0.0 : state.clock - state.resource.workingUntil;
    state.resource.idle = idle > 0.0 ? idle : state.resource.idle;

    if (!controller.queuePolicy.isFull(state.queue.size())) {
        state.queue.push({ event.entityRef, event.time,  0.0, 0.0, 0.0 });
    }

    /* generate next arrival */
    auto tec = controller.tecGenerator();
    auto arrival = state.clock + tec;
    state.events.push_back({ EventType::Arrival, arrival, ++state.entityIndexer });
}

auto simulation::consumeDeparture(Event const&, State &state, SimulationController const&) -> void {
    Entity* entity = state.resource.currentEntity;

    entity->departure = state.clock;

    StateStatistics statistics {
        *entity,                                    /* entity */
        entity->processStart - entity->arrival,     /* waiting time */
        entity->processEnd - entity->processStart,  /* ts time */
        entity->departure - entity->arrival,        /* system time */
        state.resource.idle,                        /* resource idleness */
        state.queue.size(),                         /* state queue size */
        state.entityIndexer,                        /* state entity count */
        state.queue.size() + 1                      /* state wip count */
    };

    state.resource.working = false;
    state.resource.idle = 0.0;
    delete state.resource.currentEntity;
    state.resource.currentEntity = nullptr;

    state.statistics.push_back(statistics);
}

auto simulation::consumeEnd(Event const&, State &state, SimulationController const&) -> void {
    state.over = true;
}

auto simulation::consumeEvent(Event const& event, State &state, SimulationController const& controller) -> void {
    switch (event.type) {
    case EventType::Start:
        consumeStart(event, state, controller);
        break;
    case EventType::Arrival:
        consumeArrival(event, state, controller);
        break;
    case EventType::Departure:
        consumeDeparture(event, state, controller);
        break;
    case EventType::End:
        consumeEnd(event, state, controller);
        break;
    }
}

auto simulation::updateResource(State &state, SimulationController const& controller) -> void {
    if (!state.resource.working && state.queue.size() > 0) {
        /* take an entity out of queue */
        state.resource.currentEntity = new Entity(state.queue.front());
        state.queue.pop();

        /* generate a service time */
        auto ts = controller.tsGenerator();
        state.resource.currentEntity->processStart = state.clock;
        state.resource.currentEntity->processEnd = state.clock + ts;

        state.resource.workingUntil = state.resource.currentEntity->processEnd;

        state.events.push_back(Event{
                                EventType::Departure,
                                state.resource.currentEntity->processEnd,
                                state.resource.currentEntity->id });

        /* update resource status */
        state.resource.working = true;
    }
}

auto simulation::calculate(State const& state) -> SimulationStatistics {
    SimulationStatistics ss { 0.0, 0.0, 0.0, 0.0, 0, 0 };

    size_t statsCount = state.statistics.size();

    if (statsCount > 0) {
        for (auto const& stats : state.statistics) {
            ss.averageQueueSize += stats.queueCount;
            ss.averageResourceBusy += stats.resourceIdleTime;
            ss.averageQueueTime += stats.entityWaitingTime;
            ss.averageTotalTime += stats.entitySystemTime;
            ss.entityMax = std::max(ss.entityMax, stats.wipCount);
        }

        ss.averageQueueSize /= statsCount;
        ss.averageQueueTime /= statsCount;
        ss.averageTotalTime /= statsCount;

        ss.averageResourceBusy = 1 - (ss.averageResourceBusy / state.clock);

        ss.entityOut = statsCount;
    }

    return ss;
}

auto simulation::simulate(State const& state, SimulationController const& controller) -> State {
    State newState = state;

    if (!newState.over) {
        Event iminent = advanceTime(newState);
        consumeEvent(iminent, newState, controller);
        updateResource(newState, controller);
        newState.generalStatistics.push_back(calculate(newState));

        std::stable_sort(std::begin(newState.events), std::end(newState.events), EventComparator{});
    }

    return newState;
}
