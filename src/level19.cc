#include <cassert>
#include <iostream>
#include <regex>

const std::regex RULE_ONE_RX{"(\\d+|\\|)"};

using Rules = std::map<unsigned, std::string>;
using Tests = std::vector<std::string>;

unsigned part_1(Rules rules, Tests const& tests) {

    Rules finished_rules;

    for (auto const& [k, v] : rules) {
        auto pos = v.find("\"");
        if (pos != std::string::npos) { finished_rules.insert({k, v.substr(pos + 1, 1)}); }
    }
    for (auto const& [k, v] : finished_rules) {
        rules.erase(k);
    }

    for (; !rules.empty();) {
        for (auto rit = rules.begin(); rit != rules.end();) {
            std::string rx = "(";
            for (std::sregex_iterator it(rit->second.begin(), rit->second.end(), RULE_ONE_RX), end;
                 it != end; ++it) {
                if (it->str(1) == "|") {
                    rx = rx + "|";
                    continue;
                }
                auto fit = finished_rules.find(std::stoul(it->str(1)));
                if (fit == finished_rules.end()) {
                    rx.clear();
                    break;
                }
                rx += fit->second;
            }
            if (!rx.empty()) {
                finished_rules.insert({rit->first, rx + ")"});
                rit = rules.erase(rit);
            } else {
                ++rit;
            }
        }
    }

    const std::regex r0("^" + finished_rules.at(0) + "$");

    unsigned count = 0;

    for (auto const& line : tests) {
        std::smatch m;
        if (std::regex_match(line, m, r0)) {
            ++count;
        }
    }

    return count;
}


std::string recursive_rx(Rules const& rules, unsigned rule, unsigned depth) {
    if (depth == 15) {
        return {};
    }

    auto const& r = rules.at(rule);
    if (r.front() == '"') {
        return r.substr(1, 1);
    }

    std::string rx;
    for (std::sregex_iterator it(r.begin(), r.end(), RULE_ONE_RX), end;
         it != end; ++it) {
        if (it->str(1) == "|") {
            rx = rx + "|";
            continue;
        }
        rx.append(recursive_rx(rules, std::stoul(it->str(1)), depth+1));
    }
    return "(" + rx + ")";
}


unsigned part_2(Rules rules, Tests const& tests) {

    rules[8] = "42 | 42 8";
    rules[11] = "42 31 | 42 11 31";

    std::string rx = recursive_rx(rules, 0, 0);
    const std::regex r0("^" + rx + "$");

    unsigned count = 0;

    for (auto const& line : tests) {
        std::smatch m;
        if (std::regex_match(line, m, r0)) { ++count; }
    }

    return count;
}

int main(int argc, char* argv[]) {
    Rules rules;
    Tests tests;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        auto colon = line.find(':');
        long rule = std::stoul(line.substr(0, colon));
        rules.insert({rule, line.substr(colon + 2)});
    }
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        tests.push_back(std::move(line));
    }


    std::cout << "1 Result=" << part_1(rules, tests) << "\n";
    std::cout << "2 Result=" << part_2(rules, tests) << "\n";

    return 0;
}
