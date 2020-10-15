#ifndef PARALLELSIMULATOR_HPP
#define PARALLELSIMULATOR_HPP

#include <simulator/Simulation.hpp>
#include <QThread>

class ParallelSimulator : public QThread
{
    Q_OBJECT
public:
    ParallelSimulator(simulation::Model model, simulation::NaturalNumber sleepTime);
    ~ParallelSimulator() override;

public slots:
    void simulate();

    void updateSleepTime(int sleepTime);

signals:
    void newState(simulation::Model const& model);
    void finished(simulation::Model const& model);

private:
    simulation::Model model;
    simulation::NaturalNumber sleepTime;

    auto run() -> void override;
};

#endif // PARALLELSIMULATOR_HPP
