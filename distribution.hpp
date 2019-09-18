#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>
#include <set>

#include <cmath>

#include <iostream>

namespace distribution {
    const double pi = std::atan(1) * 4;

    auto random() -> double {
        return rand() / static_cast<double>(RAND_MAX);
    }

    template <typename D>
    auto random_interval(D distribution, size_t quantity) -> std::vector<double> {
        std::vector<double> interval;
        for(size_t i = 0; i < quantity; i++) {
            interval.push_back(distribution());
        }
        return interval;
    }

    /* lambda: [capture](params) -> type { body } */

    auto uniform(double min, double max) -> std::function<double()> {
        return [min, max] () -> double {
            return (min + (max - min) * random());
        };
    }

    auto triangular(double min, double max, double mode) -> std::function<double()> {
        return [min, max, mode] () -> double {
            double u = random();

            return (u < ((mode - min) / (max - min)))
                    ? (min + std::sqrt(random() * (mode - min) * (max - min)))
                    : (max - std::sqrt((1.0 - u) * (max - mode) * (max - min)));
        };
    }

    auto exponential(double mi, double x0 = 0.0) -> std::function<double()> {
        double alpha = 1.0 / (mi - x0);

        return [alpha, x0] ()  -> double {
            return x0 - (1 / alpha) * std::log(random());
        };
    }

    auto normal(double sigma, double mi = 0.0) -> std::function<double()> {
        return [sigma, mi] () -> double {
            double z = std::sqrt(-2.0 * std::log(random())) * std::sin(2.0 * pi * random());
            return mi + sigma * z;
        };
    }

    auto histogram(std::vector<double> data) -> std::map<double, size_t> {
        std::map<double, size_t> histogram;

        /* define minmax and bucket size */
        auto const p = std::minmax_element(std::begin(data), std::end(data));
        auto const size = std::floor(std::sqrt(data.size()));
        auto const bucketLength = (*p.second - *p.first) / size;

        /* create buckets */
        auto begin = *p.first;

        std::set<std::pair<double, double>> buckets;
        for (size_t i = 0; i < size; i++) {
            auto end = begin + bucketLength;
            buckets.insert(std::make_pair(begin, end));
            begin = end;
        }

        /* count frequency */
        for (auto const& bucket : buckets) {
            auto count = std::count_if(std::begin(data), std::end(data), [&bucket](double const& d) {
                return d >= bucket.first && d <= bucket.second;
            });
            histogram.insert({(bucket.first + bucket.second) / 2.0, count});
        }

        return histogram;
    }

    auto smooth(std::vector<double> data) -> std::vector<double> {
        auto gauss = [](double const x, double const mi, double const sigma) -> double {
            auto divider = sigma * std::sqrt(2.0 * pi);
            auto exponent = -(std::pow((x - mi), 2) / (2.0 * std::pow(sigma, 2)));
            return (1 / divider * std::exp(exponent));
        };

        int const size = 5;
        int const half = size / 2;

        std::vector<double> kernel, normalizedKernel(size);
        for (int i = -half; i <= half; i++) {
            kernel.push_back(gauss(i, 0, 1));
        }

        /* sum the kernel */
        auto sum = std::accumulate(std::begin(kernel), std::end(kernel), 0.0, std::plus<double>{});

        /* normalize the kernel, so it sums up to 1 */
        std::transform(std::begin(kernel), std::end(kernel), std::begin(normalizedKernel), [sum](double const& d) -> double {
            return d / sum;
        });

        std::vector<double> smoothen;

        for (size_t i = 0; i < half; i++) {
            smoothen.push_back(data.at(i));
        }

        for (size_t i = half; i < data.size() - half; i++) {
            double sum = 0.0;
            for (size_t j = i - half; j <= i + half; j++) {
                sum += data.at(j) * normalizedKernel.at(j - i + half);
            }
            smoothen.push_back(sum);
        }

        for (size_t i = data.size() - half; i < data.size(); i++) {
            smoothen.push_back(data.at(i));
        }

        return smoothen;
    }
}


#endif // DISTRIBUTION_H
