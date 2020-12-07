#include <cassert>
#include <iostream>
#include <set>
#include <string>

unsigned decode(const char lower, const char upper, unsigned max, const std::string& data) {
    unsigned low = 0;
    for (const auto& c : data) {
        unsigned half = (max - low) / 2;
        if (c == lower) {
            max -= half;
        } else if (c == upper) {
            low += half;
        } else {
            std::cout << "Invalid data! [" << c << "]\n";
            exit(1);
        }
    }
    return low;
}

unsigned decode_row(const std::string& data) {
    assert(data.size() == 7);
    return decode('F', 'B', 128, data);
}

unsigned decode_seat(const std::string& data) {
    assert(data.size() == 3);
    return decode('L', 'R', 8, data);
}

int main(int argc, char* argv[]) {
    unsigned max_id = 0;

    assert(decode_row("FBFBBFF") == 44);
    assert(decode_seat("RLR") == 5);

    std::set<unsigned> ids;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        assert(line.size() == 10);
        unsigned row = decode_row(line.substr(0, 7));
        unsigned seat = decode_seat(line.substr(7, 3));
        unsigned id = row * 8 + seat;
        if (max_id < id) {
            max_id = id;
        }
        ids.insert(id);
    }

    for (unsigned i = 0; i < max_id; ++i) {
        if (ids.count(i) == 0) { std::cout << i << "\n"; }
    }

    std::cout << "max_id = " << max_id  << "\n";

    return 0;
}
