#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

using Board = std::deque<long>;
using CBuffer = std::vector<long>;

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

void dumpc(CBuffer const& b, const std::string& separator = ",") {
    auto first = b.at(0);
    std::cout << first << separator;
    for (auto next = b.at(first); next != first; next = b[next]) {
        std::cout << next << separator;
    }
    std::cout << "\n";
}

long make_move(CBuffer& cb, long current, long min_value, long max_value) {
    const long cut_begin = current;
    const long b1 = cb.at(cut_begin);
    const long b2 = cb.at(b1);
    const long b3 = cb.at(b2);
    long cut_end = cb.at(b3);

    cb.at(cut_begin) = cut_end;

    long insert_point = cut_begin;
    for(;;) {
        --insert_point;
        if (insert_point < min_value) {
            insert_point = max_value;
        }
        if (insert_point == b1 || insert_point == b2 || insert_point == b3) {
            continue;
        }
        break;
    }

    cut_end = cb.at(insert_point);
    cb.at(insert_point) = b1;
    cb.at(b3) = cut_end;

    return cb.at(current);
}


void part_1_fast(Board b) {
    const long max_value = *std::max_element(b.cbegin(), b.cend());
    const long min_value = *std::min_element(b.cbegin(), b.cend());

    CBuffer cbuf;
    cbuf.resize(max_value + 1);

    long first = b.front();
    long last = b.front();
    b.pop_front();
    cbuf.at(0) = last;

    for (auto const& val : b) {
        cbuf.at(last) = val;
        last = val;
    }
    cbuf.at(last) = first;

    long current = first;
    for (unsigned turn = 0; turn < MAX_TURNS_1; ++turn) {
        current = make_move(cbuf, current, min_value, max_value);
        cbuf.at(0) = current;
    }

    cbuf.at(0) = cbuf.at(1);
    std::cout << "1 Result=";
    dumpc(cbuf, "");
}

void part_2_fast(Board b) {
    const long max_value = *std::max_element(b.cbegin(), b.cend());
    const long min_value = *std::min_element(b.cbegin(), b.cend());

    CBuffer cbuf;
    cbuf.resize(MAX_VALUE_2 + 1);

    long first = b.front();
    long last = b.front();
    b.pop_front();
    cbuf.at(0) = last;

    for (auto const& val : b) {
        cbuf.at(last) = val;
        last = val;
    }
    for (long val = max_value + 1; val <= MAX_VALUE_2; ++val) {
        cbuf.at(last) = val;
        last = val;
    }
    cbuf.at(last) = first;

    long current = first;
    for (long turn = 0; turn < MAX_TURNS_2; ++turn) {
        current = make_move(cbuf, current, min_value, MAX_VALUE_2);
    }

    long b1 = cbuf.at(1);
    long b2 = cbuf.at(b1);
    std::cout << "2 Result=" << b1*b2 << "(" << b1 << ", " << b2 << ")\n";
}

int main() {
    // Board btest{3, 8, 9, 1, 2, 5, 4, 6, 7};
    Board b{2, 1, 9, 7, 4, 8, 3, 6, 5};

    part_1_fast(b);
    part_2_fast(b);

    return 0;
}
