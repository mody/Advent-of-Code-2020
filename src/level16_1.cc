#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

static const std::regex CLASS_RX {"^([^:]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9-]+)$"};

int main(int argc, char* argv[]) {

    std::vector<bool> valid_values;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        std::smatch m;
        if (!std::regex_match(line, m, CLASS_RX)) {
            std::cout << "UNEXPECTED STRING [" << line << "] at " << __LINE__ << "\n";
            exit(1);
        }

        auto fill_valid_values = [&valid_values](unsigned min, unsigned max) {
            if (valid_values.size() < max + 1) { valid_values.resize(max + 1); }
            for (unsigned i = min; i <= max; ++i) {
                valid_values[i] = true;
            }
        };
        fill_valid_values(std::stoul(m.str(2)), std::stoul(m.str(3)));
        fill_valid_values(std::stoul(m.str(4)), std::stoul(m.str(5)));
    }

    std::getline(std::cin, line);
    if (line != "your ticket:") {
        std::cout << "UNEXPECTED STRING [" << line << "] at " << __LINE__ << "\n";
        exit(1);
    }

    std::getline(std::cin, line);
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    if (line != "nearby tickets:") {
        std::cout << "UNEXPECTED STRING [" << line << "] at " << __LINE__ << "\n";
        exit(1);
    }

    std::vector<unsigned> invalid_values;

    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<std::string> values;
        boost::algorithm::split(values, line, boost::algorithm::is_any_of(","));

        for (auto const& v : values) {
            unsigned i = std::stoul(v);
            try {
                if (!valid_values.at(i)) { invalid_values.push_back(i); }
            }
            catch (std::out_of_range const& e) {
                invalid_values.push_back(i);
            }
        }
    }

    const uint64_t sum = std::accumulate(
        invalid_values.begin(), invalid_values.end(), 0U,
        [](unsigned a, auto const& v) { return a + v; });

    std::cout << "1 Result=" << sum << "\n";

    return 0;
}
