#ifndef STATE_HPP
#define STATE_HPP

#include <QCoreApplication>
#include <vector>
#include <queue>

#include "Distribution.hpp"
#include "QueuePolicy.hpp"
#include "Types.hpp"
#include "Entity.hpp"
#include "Resource.hpp"
#include "Event.hpp"

namespace simulation {
    struct StateStatistics {
        /* entity */
        Entity entity;

        TimeUnit entityWaitingTime;
        TimeUnit entityServiceTime;
        TimeUnit entitySystemTime;

        /* resource */
        TimeUnit resourceIdleTime;

        /* system */
        size_t queueCount;
        size_t entityCount;
        size_t wipCount;
    };

    struct SimulationStatistics {
        RealNumber averageQueueSize;
        RealNumber averageResourceBusy;
        RealNumber averageQueueTime;
        RealNumber averageTotalTime;
        NaturalNumber entityOut;
        NaturalNumber entityMax;
    };

    struct State {
        TimeUnit clock;
        Resource resource;
        size_t entityIndexer;
        bool over;

        std::queue<Entity> queue;
        std::vector<Event> events;

        std::vector<StateStatistics> statistics;
        std::vector<SimulationStatistics> generalStatistics;

        Event iminent;
    };

    struct SimulationController {
        using RandomGenerator = distribution::types::RandomGenerator;

        RandomGenerator tecGenerator;
        RandomGenerator tsGenerator;
        QueuePolicy queuePolicy;
    };

    struct Model {
        State state;
        SimulationController controller;
    };
}

Q_DECLARE_METATYPE(simulation::Model)

#endif // STATE_HPP
