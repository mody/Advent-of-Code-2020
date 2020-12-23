#include <algorithm>
#include <array>
#include <cstring>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <execution>
#include <vector>

using Board = std::deque<long>;
using Buffer = std::array<long, 3>;

constexpr long MAX_TURNS_1 = 100;
constexpr long MAX_TURNS_2 = 10000000;

constexpr long MAX_VALUE_2 = 1000000;

template<typename T>
void dump(T const& b, const std::string& separator = ",") {
    for (auto const& v : b) {
        std::cout << v << separator;
    }
    std::cout << "\n";
}

void replace3(Board& b, Board::iterator dst) {
    auto dist = std::distance(b.begin(), dst) - 2;

    auto src = std::next(b.begin());
    Board a(src, std::next(src, 3));

    b.erase(src, std::next(b.begin(), 4));
    b.insert(std::next(b.begin(), dist), a.begin(), a.end());
}

void rotate1(Board& b) {
    auto i = b.front();
    b.pop_front();
    b.push_back(i);
}


void part_1(Board b) {
    const long max_value = *std::max_element(b.cbegin(), b.cend());
    const long min_value = *std::min_element(b.cbegin(), b.cend());

    Board::iterator dst;
    for ( unsigned turn = 0; turn < MAX_TURNS_1; ++turn ) {
        long i = b.front();
        do {
            --i;
            if ( i < min_value ) {
                i = max_value;
            }
            if (i == b.at(1) || i == b.at(2) || i == b.at(3)) {
                dst = b.end();
            } else {
                dst = std::find(std::next(b.begin(), 4), b.end(), i);
            }
        } while(dst == b.end());
        replace3(b, dst);
        std::rotate(b.begin(), std::next(b.begin()), b.end());
    }

    std::rotate(b.begin(), std::find(b.begin(), b.end(), 1), b.end());
    b.erase(b.begin());

    std::cout << "1 Result=";
    dump(b, "");
}


void part_2(Board b) {
    const long min_value = *std::min_element(b.cbegin(), b.cend());
    long max_value = *std::max_element(b.cbegin(), b.cend());

    for ( long i = max_value + 1; i <= MAX_VALUE_2; ++i ) {
        b.push_back(i);
    }
    max_value = MAX_VALUE_2 + 1;

    std::cout << "Part 2 start\n";
    Board::iterator dst;
    for ( long turn = 0; turn < MAX_TURNS_2; ++turn ) {
        long i = b.front();
        do {
            --i;
            if ( i < min_value ) {
                i = max_value;
            }
            if (i == b.at(1) || i == b.at(2) || i == b.at(3)) {
                dst = b.end();
            } else if (i == b.front()) {
                dst = b.begin();
                break;
            } else {
                dst = std::find(std::execution::par, std::next(b.begin(), 4), b.end(), i);
            }
        } while(dst == b.end());

        replace3(b, dst);
        rotate1(b);
    }
    std::cout << "\n";

    std::rotate(b.begin(), std::find(b.begin(), b.end(), 1), b.end());

    std::cout << "2 Result=" << (b[1]*b[2]) << "\n"
        << "1: " << b[1] << "\n" << "2: " << b[2] << "\n";
}


int main() {
    Board btest{3, 8, 9, 1, 2, 5, 4, 6, 7};
    Board b{2, 1, 9, 7, 4, 8, 3, 6, 5};

    part_1(b);
    part_2(b);

    return 0;
}
