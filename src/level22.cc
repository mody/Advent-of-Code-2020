#include <boost/container_hash/hash.hpp>

#include <deque>
#include <iostream>
#include <numeric>
#include <string>

using Player = std::deque<unsigned>;

namespace std {

template <>
struct hash<Player>
{
    size_t operator()(Player const& p) const noexcept {
        size_t seed = 0;
        for ( auto const& n : p ) {
            boost::hash_combine(seed, n);
        }
        return seed;
    }
};

}  // namespace std

size_t phash(Player const& p1, Player const& p2) {
    size_t seed = 0;
    boost::hash_combine(seed, std::hash<Player>{}(p1));
    boost::hash_combine(seed, std::hash<Player>{}(p2));
    return seed;
}


unsigned long part_1(Player p1, Player p2) {

    for (;!p1.empty() && !p2.empty();) {
        auto m1 = p1.front();
        auto m2 = p2.front();

        p1.pop_front();
        p2.pop_front();

        if (m1 > m2) {
            p1.push_back(m1);
            p1.push_back(m2);
        } else {
            p2.push_back(m2);
            p2.push_back(m1);
        }
    }

    unsigned mul = 0;
    Player const& p = p1.empty() ? p2 : p1;
    return std::accumulate(p.crbegin(), p.crend(), 0UL, [&mul](auto const& cur, auto const& v) { ++mul; return cur + v * mul; });
}


Player sub_range(Player const& p, unsigned length) {
    return {p.cbegin(), std::next(p.cbegin(), length)};
}


// true -> p1, false -> p2, winner's score
std::pair<bool, unsigned long> sub_game(Player p1, Player p2) {
    std::set<size_t> visited;

    for (;!p1.empty() && !p2.empty();) {
        if (!visited.insert(phash(p1, p2)).second) {
            // break the cycle
            return {true, 0};
        }

        auto m1 = p1.front();
        auto m2 = p2.front();

        p1.pop_front();
        p2.pop_front();

        if (m1 <= p1.size() && m2 <= p2.size()) {
            if (sub_game(sub_range(p1, m1), sub_range(p2, m2)).first) {
                p1.push_back(m1);
                p1.push_back(m2);
            } else {
                p2.push_back(m2);
                p2.push_back(m1);
            }
        } else if (m1 > m2) {
            p1.push_back(m1);
            p1.push_back(m2);
        } else {
            p2.push_back(m2);
            p2.push_back(m1);
        }
    }

    unsigned mul = 0;
    Player const& p = p2.empty() ? p1 : p2;
    return {p2.empty(), std::accumulate(p.crbegin(), p.crend(), 0UL, [&mul](auto const& cur, auto const& v) {
                ++mul;
                return cur + v * mul;
            })};
}


unsigned long part_2(Player p1, Player p2) {

    return sub_game(p1, p2).second;
}

int main(int argc, char* argv[])
{
    auto parse_player = []() {
        Player p;
        std::string line;
        while ( std::getline(std::cin, line) ) {
            if ( line.empty() ) {
                break;
            }
            if ( line.substr(0, 7) == "Player " ) {
                continue;
            }
            p.push_back(std::stoi(line));
        }
        return p;
    };

    auto p1 = parse_player();
    auto p2 = parse_player();

    auto r1 = part_1(p1, p2);
    std::cout << "1 Result=" << r1 << "\n";

    auto r2 = part_2(p1, p2);
    std::cout << "2 Result=" << r2 << "\n";


    return 0;
}
