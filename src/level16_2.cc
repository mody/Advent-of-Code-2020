#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

static const std::regex CLASS_RX {"^([^:]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9-]+)$"};

struct Rule
{
    std::string name;
    unsigned min1 = 0, max1 = 0, min2 = 0, max2 = 0;
};

int main(int argc, char* argv[])
{
    std::vector<Rule> rules;
    std::vector<bool> valid_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) { break; }
        std::smatch m;
        if (!std::regex_match(line, m, CLASS_RX)) {
            std::cout << "UNEXPECTED STRING [" << line << "] at " << __LINE__ << "\n";
            exit(1);
        }

        Rule r;
        r.name = m.str(1);
        r.min1 = std::stoul(m.str(2));
        r.max1 = std::stoul(m.str(3));
        r.min2 = std::stoul(m.str(4));
        r.max2 = std::stoul(m.str(5));
        rules.push_back(std::move(r));

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

    std::vector<std::string> tickets;

    std::getline(std::cin, line);
    tickets.push_back(line);

    std::getline(std::cin, line);
    std::getline(std::cin, line);
    if (line != "nearby tickets:") {
        std::cout << "UNEXPECTED STRING [" << line << "] at " << __LINE__ << "\n";
        exit(1);
    }

    while (std::getline(std::cin, line)) {
        if (line.empty()) { break; }
        tickets.push_back(line);
    }

    std::map<unsigned, std::map<std::string, unsigned>> stats;
    unsigned rows = 0;

    for (auto const& line : tickets) {
        std::vector<std::string> values;
        boost::algorithm::split(values, line, boost::algorithm::is_any_of(","));

        // check for valid row first
        bool is_valid = true;
        for (auto const& v : values) {
            unsigned i = std::stoul(v);
            try {
                if (!valid_values.at(i)) { is_valid = false; }
            }
            catch (std::out_of_range const& e) {
                is_valid = false;
            }
        }
        if (!is_valid) { continue; }
        ++rows;

        for (size_t pos = 0; pos < values.size(); ++pos) {
            unsigned i = std::stoul(values.at(pos));
            for (auto const& r : rules) {
                if ((i >= r.min1 && i <= r.max1) || (i >= r.min2 && i <= r.max2)) {
                    stats[pos][r.name] += 1;
                }
            }
        }
    }

    std::map<std::string, unsigned> name_pos;

    for (auto it = stats.begin(); it != stats.end();) {
        unsigned top_count = 0;
        for (auto const& [k, v] : it->second) {
            if (v == rows) { ++top_count; }
        }
        if (top_count == 1) {
            // have a winner rule
            for (auto const& [k, v] : it->second) {
                if (v == rows) { name_pos[k] = it->first; }
            }

            // Clear stats from the winner.
            for (auto it = stats.begin(); it != stats.end(); ++it) {
                auto& m = it->second;
                for (auto it2 = m.begin(); it2 != m.end();) {
                    if (name_pos.count(it2->first)) {
                        it2 = m.erase(it2);
                    } else {
                        ++it2;
                    }
                }
            }
            it = stats.begin();
        } else {
            ++it;
        }
    }

    std::vector<std::string> values;
    boost::algorithm::split(values, tickets.front(), boost::algorithm::is_any_of(","));

    unsigned long result = 1;
    for (auto const& [name, pos] : name_pos) {
        if (!boost::algorithm::starts_with(name, "departure")) { continue; }
        result *= std::stoul(values.at(pos));
    }

    std::cout << "2 Result=" << result << "\n";

    return 0;
}
