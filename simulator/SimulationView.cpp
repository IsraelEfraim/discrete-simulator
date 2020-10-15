#include "SimulationView.hpp"

auto simulation::view::makeCalendar(std::vector<Event> const& events) -> std::vector<FutureEvent> {
    std::vector<FutureEvent> calendar;

    for (auto const& event : events) {
        calendar.push_back({ event.entityRef, event.time, event.type });
    }

    return calendar;
}

auto simulation::view::makeDataRow(State const& state) -> DataRow {
    DataRow dr {
        state.iminent.entityRef,
        state.clock,
        state.iminent.type,
        state.queue.size(),
        {
            state.iminent.type == EventType::Arrival,
            state.iminent.type == EventType::Arrival
            ? state.iminent.time
            : 0.0
        },
        makeCalendar(state.events),
        state.generalStatistics.back()
    };

    return dr;
}

auto simulation::view::getCalendar(std::vector<FutureEvent> const& events) -> std::string {
    std::string calendar{""};

    for (auto const& e : events) {
        calendar += "(" + std::to_string(e.entityRef) + ", " + std::to_string(e.time)
                    + ", " + getEventTypeName(e.eventType) + ")\n";
    }

    return calendar;
}

auto simulation::view::getEventTypeName(EventType eventType) -> std::string {
    std::string eventName;

    switch(eventType) {
    case EventType::Start:
        eventName = "Start";
        break;
    case EventType::Arrival:
        eventName = "Arrival";
        break;
    case EventType::Departure:
        eventName = "Departure";
        break;
    case EventType::End:
        eventName = "End";
        break;
    }

    return eventName;
}
