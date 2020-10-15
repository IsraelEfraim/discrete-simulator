#include "Distribution.hpp"

const double pi = std::atan(1) * 4;

auto distribution::random() -> double {
    double u = rand() / static_cast<double>(RAND_MAX);
    return u;
}

/* lambda: [capture](params) -> type { body } */

auto distribution::uniform(double min, double max) -> types::RandomGenerator {
    return [min, max] () -> double {
        return (min + (max - min) * random());
    };
}

auto distribution::triangular(double min, double max, double mode) -> types::RandomGenerator {
    return [min, max, mode] () -> double {
        double u = random();

        return (u < ((mode - min) / (max - min)))
                   ? (min + std::sqrt(u * (mode - min) * (max - min)))
                   : (max - std::sqrt((1.0 - u) * (max - mode) * (max - min)));
    };
}

auto distribution::exponential(double mi, double x0) -> types::RandomGenerator {
    double alpha = 1.0 / (mi - x0);

    return [alpha, x0] ()  -> double {
        return x0 - (1 / alpha) * std::log(random());
    };
}

auto distribution::normal(double sigma, double mi) -> types::RandomGenerator {
    return [sigma, mi] () -> double {
        double z = std::sqrt(-2.0 * std::log(random())) * std::sin(2.0 * pi * random());
        return mi + sigma * z;
    };
}

auto distribution::empirical(std::map<double, double> const& dpf) -> types::RandomGenerator {
    return [dpf] () -> double {
        double u = random();

        auto pair =  std::find_if(std::begin(dpf), std::end(dpf), [u] (types::DensityPair const& p) -> bool {
            return u <= p.first;
        });

        return pair->second;
    };
}

auto distribution::guard(types::RandomGenerator distribution) -> types::RandomGenerator {
    return [distribution] () -> double {
        double u = distribution();

        while (u < 0.0) {
            u = distribution();
        }

        return u;
    };
}

auto distribution::histogram(std::vector<double> data) -> std::map<double, size_t> {
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

auto distribution::smooth(std::vector<double> data) -> std::vector<double> {
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
