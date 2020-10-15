#ifndef SIMULATIONVIEW_HPP
#define SIMULATIONVIEW_HPP

#include "Simulation.hpp"

namespace simulation {
namespace view {
    struct FutureEvent {
        size_t entityRef;
        TimeUnit time;
        EventType eventType;
    };

    struct DataRow {
        size_t entityId;
        TimeUnit clock;
        EventType eventType;
        size_t queueSize;
        std::pair<bool, double> tc;
        std::vector<FutureEvent> calendar;
        SimulationStatistics generalStatistics;
    };

    auto makeCalendar(std::vector<Event> const& events) -> std::vector<FutureEvent>;

    auto makeDataRow(State const& state) -> DataRow;

    auto getCalendar(std::vector<FutureEvent> const& events) -> std::string;

    auto getEventTypeName(EventType eventType) -> std::string;
}
}

#endif // SIMULATIONVIEW_HPP
