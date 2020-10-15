#ifndef EXPORT_HPP
#define EXPORT_HPP

#include "Simulation.hpp"

namespace simulation {
namespace persist {

    auto toFile(State const& final, std::string const& filename) -> void;

}
}

#endif // EXPORT_HPP
