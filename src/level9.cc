#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

constexpr unsigned PREAMBLE = 25;
std::deque<long> window;
std::vector<long> data;

bool check_sum(long x) {
    assert(window.size() == PREAMBLE);

    for(unsigned i = 0; i < window.size() - 1; ++i) {
        for (unsigned j = i + 1; j < window.size(); ++j) {
            if (window[i] == window[j]) {
                continue;
            }
            if (window[i] + window[j] == x) {
                return true;
            }
        }
    }
    return false;
}

std::pair<long, long> find_sum(const long incorrect)
{
    for (auto first = data.begin(); first != std::prev(data.end()); ++first) {
        long sum = *first;
        long min = *first;
        long max = *first;
        for (auto last = std::next(first); last != data.end(); ++last) {
            sum += *last;
            min = std::min(min, *last);
            max = std::max(max, *last);
            if (sum == incorrect) {
                return {min, max};
            }
            if (sum > incorrect) {
                break;
            }
        }
    }
    return {0, 0};
}

int main(int argc, char* argv[]) {
    long incorrect = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        long num = std::stol(line);
        data.push_back(num);

        if (window.size() < PREAMBLE) {
            window.push_back(num);
            continue;
        }
        assert(window.size() == PREAMBLE);
        if (!check_sum(num)) {
            incorrect = num;
            std::cout << "Incorrect number: " << num << "\n";
            break;
        }
        window.push_back(num);
        window.pop_front();
    }

    auto [min, max] = find_sum(incorrect);
    std::cout << "min=" << min << ", max=" << max << ", sum=" << (min+max) << "\n";

    return 0;
}
