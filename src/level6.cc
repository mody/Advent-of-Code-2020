#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

unsigned answers1(const std::vector<std::string>& data) {
    std::set<unsigned char> chars;
    for (const auto& c : boost::algorithm::join(data, "")) {
        chars.insert(c);
    }
    return chars.size();
}

unsigned answers(const std::vector<std::string>& data) {
    std::map<unsigned char, unsigned> chars;
    for (const auto& c : boost::algorithm::join(data, "")) {
        chars[c] += 1;
    }
    unsigned cnt = 0;
    for (const auto& [k, v] : chars) {
        if (v == data.size()) {
            ++cnt;
        }
    }
    return cnt;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> data;

    unsigned sum = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            sum += answers(data);
            data.clear();
            continue;
        }
        data.push_back(line);
    }

    sum += answers(data);

    std::cout << "Sum: " << sum << "\n";
    return 0;
}
