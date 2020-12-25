#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <unordered_set>

using Stats = std::map<std::string, unsigned>;

struct Dir {
    long dx = 0, dy = 0;
};

static std::map<std::string, Dir> DIRECTIONS{
    { "w", {-2, 0}},
    { "e", {+2, 0}},
    {"nw", {-1,+1}},
    {"ne", {+1,+1}},
    {"sw", {-1,-1}},
    {"se", {+1,-1}},
};


struct Tile {
    long x = 0, y = 0;
    mutable bool white = true;

    void move(std::string const& dir) {
        auto const& d = DIRECTIONS.at(dir);
        x += d.dx;
        y += d.dy;
    }

    void flip() const noexcept {
        white = !white;
    }

    bool operator== (Tile const& oth) const noexcept {
        return x == oth.x && y == oth.y;
    }
};

namespace std {

template<>
struct hash<Stats>
{
    size_t operator()(Stats const& s) const noexcept
    {
        size_t seed = 0;
        for (auto const& [k, v] : s) {
            boost::hash_combine(seed, k);
            boost::hash_combine(seed, v);
        }
        return seed;
    }
};

template<>
struct hash<Tile>
{
    size_t operator()(Tile const& t) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, t.x);
        boost::hash_combine(seed, t.y);
        return seed;
    }
};

}  // namespace std


Stats tokenize(std::string const& line) {
    Stats s;

    char last = ' ';
    for (auto const& c : line) {
        if (c == 's' || c == 'n') {
            last = c;
            continue;
        }
        std::string k;
        if (last != ' ') {
            k += last;
            last = ' ';
        }
        k += c;
        s.insert({k, 0}).first->second++;
    }
    return s;
}


void reduce(Stats& s) {
    auto substract = [&s](std::string const& a, std::string const& b, std::string const& res) {
        auto dif = std::min(s[a], s[b]);
        s[a] -= dif;
        s[b] -= dif;
        s[res] += dif;
        return dif != 0;
    };

    auto eliminate = [&s](std::string const& a, std::string const& b) {
        auto dif = std::min(s[a], s[b]);
        s[a] -= dif;
        s[b] -= dif;
        return dif != 0;
    };

    for (bool modified = true; modified;) {
        modified = false;
        // nw,sw -> w
        modified |= substract("nw", "sw", "w");

        // ne,se -> e
        modified |= substract("ne", "se", "e");

        // e,w -> -
        modified |= eliminate("e", "w");

        // ne,sw -> -
        modified |= eliminate("ne", "sw");

        // se,nw -> -
        modified |= eliminate("se", "nw");

        // ne = e+nw
        modified |= substract("e", "nw", "ne");

        // se = e+sw
        modified |= substract("e", "sw", "se");

        // nw = w+ne
        modified |= substract("w", "ne", "nw");

        // sw = w+se
        modified |= substract("w", "se", "sw");
    }

    // drop empty
    for (auto it = s.begin(); it != s.end();) {
        if (it->second == 0) {
            it = s.erase(it);
        } else {
            ++it;
        }
    }
}

void part_1(std::unordered_map<Stats, unsigned> const& data) {
    unsigned black = 0;
    for (auto const& [s, cnt] : data) {
        if (cnt & 0x1) { ++black; }
    }

    std::cout << "1 Result=" << black << "\n";
}

void part_2(std::unordered_map<Stats, unsigned> const& data) {

    std::unordered_set<Tile> mapa;
    for (auto const& [s, flip_cnt] : data) {
        Tile t;
        for (auto const& [m, cnt] : s) {
            for (unsigned i = 0; i < cnt; ++i) {
                t.move(m);
            }
        }
        auto it = mapa.insert(std::move(t)).first;
        for (unsigned i = 0; i < flip_cnt; ++i) {
            it->flip();
        }
    }

    auto count_blacks_around = [&mapa](Tile const& t) {
        unsigned blacks = 0;
        for (auto const& [dir, _] : DIRECTIONS) {
            Tile tt{t};
            tt.move(dir);
            auto it = mapa.find(tt);
            if (it != mapa.end() && it->white == false) {
                ++blacks;
            }
        }
        return blacks;
    };

    for (unsigned turn = 0; turn < 100; ++turn) {
        std::unordered_set<Tile> m2;

        long min_x = std::numeric_limits<long>::max();
        long min_y = std::numeric_limits<long>::max();
        long max_x = std::numeric_limits<long>::min();
        long max_y = std::numeric_limits<long>::min();
        for (auto const& t : mapa) {
            min_x = std::min(min_x, (long)t.x);
            min_y = std::min(min_y, (long)t.y);
            max_x = std::max(max_x, (long)t.x);
            max_y = std::max(max_y, (long)t.y);
        }
        --min_x;
        --min_y;
        ++max_x;
        ++max_y;

        for (long y = min_y; y <= max_y; ++y) {
            for (long x = min_x; x <= max_x; ++x) {
                Tile t{x, y};
                auto it = mapa.find(t);
                if (it != mapa.end()) {
                    t = *it;
                }

                auto blacks = count_blacks_around(t);
                if (t.white) {
                    if (blacks == 2) {
                        t.flip();
                    }
                } else {
                    if (blacks == 0 || blacks > 2) {
                        t.flip();
                    }
                }
                m2.insert(std::move(t));
            }
        }
        std::swap(mapa, m2);
    }

    unsigned black = 0;
    for (auto const& t : mapa) {
        if (!t.white) {
            ++black;
        }
    }
    std::cout << "2 Result=" << black << "\n";
        }

int main()
{
    std::unordered_map<Stats, unsigned> data;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) break;

        auto stats = tokenize(line);
        reduce(stats);
        data.insert({stats, 0}).first->second++;
    }

    part_1(data);
    part_2(data);

    return 0;
}
