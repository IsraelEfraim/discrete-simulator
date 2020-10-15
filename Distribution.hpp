#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>
#include <set>
#include <cmath>

namespace distribution {

    namespace types {
        using DensityPair = std::pair<double, double>;
        using RandomGenerator = std::function<double()>;
    }

    template <typename D>
    auto random_interval(D distribution, size_t quantity) -> std::vector<double> {
        std::vector<double> interval;
        for(size_t i = 0; i < quantity; i++) {
            interval.push_back(distribution());
        }
        return interval;
    }

    auto random() -> double;

    auto uniform(double min, double max) -> types::RandomGenerator;
    auto triangular(double min, double max, double mode) -> types::RandomGenerator;
    auto exponential(double mi, double x0 = 0.0) -> types::RandomGenerator;
    auto normal(double sigma, double mi = 0.0) -> types::RandomGenerator;
    auto empirical(std::map<double, double> const& dpf) -> types::RandomGenerator;

    auto guard(types::RandomGenerator distribution) -> types::RandomGenerator;

    auto histogram(std::vector<double> data) -> std::map<double, size_t>;
    auto smooth(std::vector<double> data) -> std::vector<double>;
}


#endif // DISTRIBUTION_H
