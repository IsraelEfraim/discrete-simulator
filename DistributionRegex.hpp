#ifndef DISTRIBUTIONREGEX_HPP
#define DISTRIBUTIONREGEX_HPP

#include "Distribution.hpp"

#include <regex>

namespace distribution {
namespace regex {
    auto match(std::string const& str) -> types::RandomGenerator;
}
}

#endif // DISTRIBUTIONREGEX_HPP
