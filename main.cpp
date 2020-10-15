#include "MainWindow.h"
#include "simulator/Simulation.hpp"

#include "DistributionRegex.hpp"
#include <exception>

#include <iostream>
#include <QApplication>

#include <regex>

auto cli_state(simulation::State& state) -> void {
    std::cout << "Clock            : " << state.clock << std::endl
              << "Queue Size       : " << state.queue.size() << std::endl
              << "Events remain.   : " << state.events.size() << std::endl
              << "Entity Count     : " << state.entityIndexer << std::endl
              << "Resource status  : " << state.resource.working << std::endl << std::endl;
}

auto simpleSimulationTest() -> void {
    using namespace simulation;

    SimulationController control { distribution::uniform(1, 10), distribution::uniform(3, 7), simulation::unlimitedQueue() };

    State state { cleanState(0.0, 600) };
    cli_state(state);

    while (!state.over) {
        state = simulate(state, control);
        cli_state(state);
    }

    std::cout << "Max " << state.generalStatistics.back().entityMax << std::endl;
    std::cout << "Out " << state.generalStatistics.back().entityOut << std::endl;
    std::cout << "AvgQS " << state.generalStatistics.back().averageQueueSize << std::endl;
    std::cout << "AvgQT " << state.generalStatistics.back().averageQueueTime << std::endl;
    std::cout << "AvgTT " << state.generalStatistics.back().averageTotalTime << std::endl;
    std::cout << "AvgRB " << state.generalStatistics.back().averageResourceBusy << std::endl;

    std::cout << "Estou aqui men" << std::endl;
}

auto simpleDistributionRegexTest() -> void {
    std::vector<std::string> perdoemeJose {
        "UNIF(10,2)",
        "UNIF(1.0,2.0)",
        "TRIA ( 1, 10, 5)",
        "DISC  ( 1.5, 0.2, 3.5,1 )",
        "EXPO(1,2)",
        "EXPO (1.0, 2.0)",
        "EXPO (10, 5.2)",
        "DISC ( 0.13, 1, 0.25, 2, 0.38, 3, 0.5, 4, 0.63, 5, 0.75, 6, 0.88, 7, 1, 8)",
        "DISC ( 0.1, 1, 0.3, 2, 0.6, 3, 0.85, 4, 0.95, 5, 1, 6)"
    };

    for (auto const& j : perdoemeJose) {
        try {
            distribution::regex::match(j);
        } catch (std::logic_error &e) {
            std::cout << e.what() << std::endl;
        }
    }
}

auto simpleTestQueuePolicy() -> void {
    simulation::QueuePolicy qp { simulation::unlimitedQueue() };
    qp.isFull(5);

    simulation::QueuePolicy uqp { simulation::limitedQueue(10) };
    uqp.isFull(9);
}

int main(int argc, char *argv[])
{
    //simpleDistributionRegexTest();
    //simpleTestQueuePolicy();
    //simpleSimulationTest();
    /*auto d = distribution::exponential(11.3, 0);

    for (size_t i = 0; i < 1000; i++) {
        std::cout << d() << std::endl;
    }*/

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
