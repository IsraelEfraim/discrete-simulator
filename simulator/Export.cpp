#include "Export.hpp"

#include <fstream>
#include <iomanip>

auto simulation::persist::toFile(State const& final, std::string const& filename) -> void {
    std::ofstream file(filename);

    auto last = final.generalStatistics.back();

    file << "[Coliseum 1.0] Simulation Results" << std::endl << std::endl;

    file << "Simulation.Clock        : " << std::setprecision(5) << final.clock << std::endl;
    file << "Entity.QueueSizeAverage : " << std::setprecision(5) << last.averageQueueSize << std::endl;
    file << "Entity.QueueTimeAverage : " << std::setprecision(5) << last.averageQueueTime << std::endl;
    file << "Entity.TotalTimeAverage : " << std::setprecision(5) << last.averageTotalTime << std::endl;
    file << "Entity.NumberMax        : " << std::setprecision(5) << last.entityMax << std::endl;
    file << "Entity.NumberIn         : " << std::setprecision(5) << final.entityIndexer << std::endl;
    file << "Entity.NumberOut        : " << std::setprecision(5) << last.entityOut << std::endl;
    file << "Resource.BusyAverage    : " << std::setprecision(5) << last.averageResourceBusy << std::endl;
    file << std::endl;
}
