#include "ParallelSimulator.hpp"

ParallelSimulator::ParallelSimulator(simulation::Model model, simulation::NaturalNumber sleepTime)
    : model(model), sleepTime(sleepTime)
{}

auto ParallelSimulator::simulate() -> void {
    while (!model.state.over) {
        model.state = simulation::simulate(model.state, model.controller);

        emit newState(model);

        if (sleepTime > 0) {
            this->msleep(static_cast<unsigned long>(sleepTime));
        }
    }
    emit finished(model);
}

auto ParallelSimulator::updateSleepTime(int sleepTime) -> void {
    this->sleepTime = sleepTime;
}

auto ParallelSimulator::run() -> void {
    this->simulate();
}

ParallelSimulator::~ParallelSimulator() = default;
