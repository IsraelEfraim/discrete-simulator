#include "DistributionRegex.hpp"

#include <exception>
#include <iostream>

namespace details {
    namespace types = distribution::types;

    auto build_uniform(std::smatch const& match) -> types::RandomGenerator {
        double min = std::stod(match.str(2)),
               max = std::stod(match.str(3));

        std::cout << "Building uniform " << min << " " << max << std::endl;

        return distribution::uniform(min, max);
    }

    auto build_triangular(std::smatch const& match) -> types::RandomGenerator {
        double min = std::stod(match.str(2)),
               mode = std::stod(match.str(3)),
               max = std::stod(match.str(4));

        std::cout << "Building triangular " << min << " " << max << " " << mode << std::endl;

        return distribution::triangular(min, max, mode);
    }

    auto build_exponential(std::smatch const& match) -> types::RandomGenerator {
        double mi = std::stod(match.str(2)),
               x0 = std::stod(match.str(3));

        std::cout << "Building exponential " << mi << " " << x0 << std::endl;

        return distribution::exponential(mi, x0);
    }

    auto build_normal(std::smatch const& match) -> types::RandomGenerator {
        double mi = std::stod(match.str(2)),
               sigma = std::stod(match.str(3));

        std::cout << "Building normal " << sigma << " " << mi << std::endl;

        return distribution::guard(distribution::normal(sigma, mi));
    }

    auto build_empirical(std::smatch const& match) -> types::RandomGenerator {
        std::map<double, double> dpf;

        std::string matchStr = match.str(0).substr(5, match.str(0).length() - 6);
        size_t lbound = 0, mbound = 0, ubound = 0;

        while (ubound < std::string::npos) {
            mbound = matchStr.find(',', lbound);
            ubound = matchStr.find(',', mbound + 1);

            if (mbound < std::string::npos) {
                double p = std::stod(matchStr.substr(lbound, mbound - lbound)),
                       v = std::stod(matchStr.substr(mbound + 1, ubound - mbound - 1));

                dpf.insert({ p, v });

                if (ubound != std::string::npos)
                    lbound = ubound + 1;
            }
        }

        std::cout << "Building discrete with " << dpf.size() << " entries " << std::endl;

        return distribution::empirical(dpf);
    }
}

auto distribution::regex::match(std::string const& str) -> types::RandomGenerator {
    std::vector<std::regex> regexes {
        std::regex {"(UNIF)\\(([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?)\\)"},
        std::regex {"(TRIA)\\(([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?)\\)"},
        std::regex {"(EXPO)\\(([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?)\\)"},
        std::regex {"(NORM)\\(([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?)\\)"},
        std::regex {"(DISC)\\(([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?)(,([0-9]+(?:\\.[0-9]+)?),([0-9]+(?:\\.[0-9]+)?))*\\)"}
    };

    std::string formatted = std::regex_replace(str, std::regex("\\s+"), "");
    std::smatch results;

    for (auto const& r : regexes) {
        if (std::regex_match(formatted, results, r)) {
            std::string name = results.str(1);

            types::RandomGenerator rg;

            if (name == "UNIF") {
                rg = details::build_uniform(results);
            }
            else if (name == "TRIA") {
                rg = details::build_triangular(results);
            }
            else if (name == "EXPO") {
                rg = details::build_exponential(results);
            }
            else if (name == "NORM") {
                rg = details::build_normal(results);
            }
            else if (name == "DISC") {
                rg = details::build_empirical(results);
            }

            return rg;
        }
    }

    throw std::logic_error("Regex match failed (distribution not supported).");
}
