#include <boost/container_hash/hash.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    long x = 0, y = 0, z = 0, w = 0;

    bool operator==(Point const& o) const noexcept { return x == o.x && y == o.y && z == o.z && w == o.w; }
};

Point operator+(Point lhs, Point const& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

std::vector<Point> OFFSETS3D;
std::vector<Point> OFFSETS4D;

namespace std {

template <>
struct hash<Point>
{
    size_t operator()(Point const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        boost::hash_combine(seed, p.z);
        return seed;
    }
};

}  // namespace std


using Mapa = std::vector<std::string>;
using World = std::unordered_set<Point>;


unsigned surrounded_by(World const& world, Point const& center, std::vector<Point> const& offsets) noexcept
{
    unsigned count = 0;
    for (Point const& off : offsets) {
        if (world.count(center+off)) {
            ++count;
        }
    }
    return count;
}


auto measure(World const& world) {
    long min_x = std::numeric_limits<long>::max();
    long min_y = std::numeric_limits<long>::max();
    long min_z = std::numeric_limits<long>::max();
    long min_w = std::numeric_limits<long>::max();
    long max_x = std::numeric_limits<long>::min();
    long max_y = std::numeric_limits<long>::min();
    long max_z = std::numeric_limits<long>::min();
    long max_w = std::numeric_limits<long>::min();
    for ( auto const& p : world ) {
        min_x = std::min(min_x, p.x);
        min_y = std::min(min_y, p.y);
        min_z = std::min(min_z, p.z);
        min_w = std::min(min_w, p.w);
        max_x = std::max(max_x, p.x);
        max_y = std::max(max_y, p.y);
        max_z = std::max(max_z, p.z);
        max_w = std::max(max_w, p.w);
    }
    --min_x;
    --min_y;
    --min_z;
    --min_w;
    ++max_x;
    ++max_y;
    ++max_z;
    ++max_w;
    return std::make_tuple(min_x, max_x, min_y, max_y, min_z, max_z, min_w, max_w);
}


unsigned part_1(Mapa const& mapa)
{
    World world;

    long min_x = 0, min_y = 0, min_z = 0, min_w = 0;
    long max_x = 0, max_y = 0, max_z = 0, max_w = 0;
    long x = 0, y = 0, z = 0;

    for (auto const& row : mapa) {
        x = 0;
        for (auto const& c : row) {
            if (c == '#') {
                world.insert({x, y});
            }
            ++x;
        }
        y++;
    }

    for ( int i = 0; i < 6; ++i ) {
        World new_world;
        std::tie(min_x, max_x, min_y, max_y, min_z, max_z, min_w, max_w) = measure(world);

        for ( z = min_z; z <= max_z; ++z ) {
            for ( y = min_y; y <= max_y; ++y ) {
                for ( x = min_x; x <= max_x; ++x ) {
                    Point p{x, y, z};
                    const bool active = world.count(p);
                    const unsigned around = surrounded_by(world, p, OFFSETS3D);
                    if (active) {
                       if (!(around == 2 || around == 3)) {
                           // inactive
                           continue;
                       }
                       // stays active
                       new_world.insert(std::move(p));
                    } else {
                        if ( around == 3 ) {
                            // becomes active
                            new_world.insert(std::move(p));
                        }
                    }

                }
            }
        }
        std::swap(world, new_world);
    }

    return world.size();
}


unsigned part_2(Mapa const& mapa)
{
    World world;

    long min_x = 0, min_y = 0, min_z = 0, min_w = 0;
    long max_x = 0, max_y = 0, max_z = 0, max_w = 0;
    long x = 0, y = 0, z = 0, w = 0;

    for (auto const& row : mapa) {
        x = 0;
        for (auto const& c : row) {
            if (c == '#') {
                world.insert({x, y});
            }
            ++x;
        }
        y++;
    }

    for ( int i = 0; i < 6; ++i ) {
        World new_world;
        std::tie(min_x, max_x, min_y, max_y, min_z, max_z, min_w, max_w) = measure(world);

        for ( w = min_w; w <= max_w; ++w ) {
            for ( z = min_z; z <= max_z; ++z ) {
                for ( y = min_y; y <= max_y; ++y ) {
                    for ( x = min_x; x <= max_x; ++x ) {
                        Point p{x, y, z, w};
                        const bool active = world.count(p);
                        const unsigned around = surrounded_by(world, p, OFFSETS4D);
                        if (active) {
                        if (!(around == 2 || around == 3)) {
                            // inactive
                            continue;
                        }
                        // stays active
                        new_world.insert(std::move(p));
                        } else {
                            if ( around == 3 ) {
                                // becomes active
                                new_world.insert(std::move(p));
                            }
                        }

                    }
                }
            }
        }
        std::swap(world, new_world);
    }

    return world.size();
}


int main(int argc, char* argv[]) {
    Mapa mapa;

    const auto offsets = {-1, 0, 1};
    for (auto iz = offsets.begin(); iz != offsets.end(); ++iz) {
        for (auto iy = offsets.begin(); iy != offsets.end(); ++iy) {
            for (auto ix = offsets.begin(); ix != offsets.end(); ++ix) {
                if (*ix == 0 && *iy == 0 && *iz == 0) { continue; }
                OFFSETS3D.push_back({*ix, *iy, *iz});
            }
        }
    }
    assert(OFFSETS3D.size() == 26);

    for (auto iw = offsets.begin(); iw != offsets.end(); ++iw) {
        for (auto iz = offsets.begin(); iz != offsets.end(); ++iz) {
            for (auto iy = offsets.begin(); iy != offsets.end(); ++iy) {
                for (auto ix = offsets.begin(); ix != offsets.end(); ++ix) {
                    if (*ix == 0 && *iy == 0 && *iz == 0 && *iw == 0) { continue; }
                    OFFSETS4D.push_back({*ix, *iy, *iz, *iw});
                }
            }
        }
    }
    assert(OFFSETS4D.size() == 80);


    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        mapa.push_back(line);
    }

    std::cout << "1 Result=" << part_1(mapa) << "\n";
    std::cout << "2 Result=" << part_2(mapa) << "\n";

    return 0;
}
