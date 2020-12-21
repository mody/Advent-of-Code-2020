#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <vector>

using Lines = std::vector<std::string>;
using Side = std::map<size_t, unsigned>;

using shash = std::hash<std::string>;


void flip_diagonal(Lines& image) {
    Lines i2     = image;
    unsigned col = 0;
    for ( auto& l2 : image )
    {
        l2.clear();
        for ( auto const& l : i2 )
        {
            l2 += l.at(col);
        }
        ++col;
    }
}

void flip_ud(Lines& image) {
    for (unsigned col = 0; col < image.front().size(); ++col) {
        for (unsigned row = 0; row < image.size() / 2; ++row) {
            std::swap(image[row][col], image[image.size() - row - 1][col]);
        }
    }
}

void flip_lr(Lines& image) {
    for ( auto& l : image ) {
        std::reverse(l.begin(), l.end());
    }
}

void rotate_cw_90(Lines& image) {
    flip_diagonal(image);
    flip_lr(image);
}

void rotate_ccw_90(Lines& image) {
    flip_diagonal(image);
    flip_ud(image);
}


struct Tile {
    unsigned id = 0;
    unsigned used_sides = 0;
    int x = -9999, y = -9999;
    Side all_sides;
    std::string left, right, top, bottom;
    Lines image;

    bool used = false;

    void flip_diagonal() {
        ::flip_diagonal(image);
        std::swap(top, left);
        std::swap(right, bottom);
    }

    void flip_ud() {
        ::flip_ud(image);
        std::reverse(left.begin(), left.end());
        std::reverse(right.begin(), right.end());
        std::swap(top, bottom);
    }

    void flip_lr() {
        ::flip_lr(image);
        std::reverse(top.begin(), top.end());
        std::reverse(bottom.begin(), bottom.end());
        std::swap(left, right);
    }

    void rotate_cw_90() {
        flip_diagonal();
        flip_lr();
    }

    void rotate_ccw_90() {
        flip_diagonal();
        flip_ud();
    }

    static Tile parse(Lines const& _lines) {
        assert(_lines.size() == 11);
        assert(_lines.front().substr(0, 5) == "Tile ");

        Tile t;
        t.image = _lines;
        t.id = std::stoul(t.image.front().substr(5));
        t.image.erase(t.image.begin()); // drop Tile id line

        t.top = t.image.front();
        t.bottom = t.image.back();
        for (auto const& l : t.image) {
            t.left += l.front();
            t.right += l.back();
        }

        // strip the borders
        t.image.erase(t.image.begin());
        t.image.erase(std::prev(t.image.end()));
        for (auto& l : t.image) {
            l.erase(l.begin());
            l.erase(std::prev(l.end()));
        }

        auto compute_side = [&t](std::string data) {
            t.all_sides.insert({shash {}(data), 0});
            std::reverse(data.begin(), data.end());
            t.all_sides.insert({shash {}(data), 0});
        };

        compute_side(t.top);
        compute_side(t.bottom);
        compute_side(t.left);
        compute_side(t.right);

        return t;
    }
};

using Tiles = std::vector<Tile>;


unsigned long long part_1(Tiles tiles) {

    std::map<size_t, unsigned> known_sides;
    for (auto const& t : tiles) {
        for (auto const& [s, cnt] : t.all_sides) {
            known_sides.insert({s, 0}).first->second++;
        }
    }

    for (auto& t : tiles) {
        for (auto const& [s, cnt] : t.all_sides) {
            if (known_sides.at(s) > 1) {
                ++t.used_sides;
            }
        }
    }

    unsigned long long result = 1;

    for (auto const& t : tiles) {
        if (t.used_sides < 5) {
            result *= t.id;
        }
    }

    return result;
}

void connect_tile(Tile& me, Tiles& tiles, std::set<unsigned>& visited)
{
    if(!visited.insert(me.id).second) {
        std::cout << "Going circles! Already visited id " << me.id << ", used: " << me.used << "\n";
        return;
    }

    me.used = true;
    if (tiles.empty()) { return ; }

    auto check_match = [&me, &tiles, &visited](Tile& t) {
        assert(me.id != t.id);
        assert(t.used == false);

        auto deeper = [&me, &tiles, &visited](Tile& t, std::string& me_side, std::string& t_side) {
            me_side.clear();
            t_side.clear();
            connect_tile(t, tiles, visited);
        };

        bool connected = false;

        if (!me.right.empty() && me.right == t.left) {
            t.y = me.y;
            t.x = me.x + 1;
            deeper(t, me.right, t.left);
            connected = true;
        }

        if (!me.bottom.empty() && me.bottom == t.top) {
            t.x = me.x;
            t.y = me.y + 1;
            deeper(t, me.bottom, t.top);
            connected = true;
        }

        if (!me.left.empty() && me.left == t.right) {
            t.x = me.x - 1;
            t.y = me.y;
            deeper(t, me.left, t.right);
            connected = true;
        }

        if (!me.top.empty() && me.top == t.bottom) {
            t.x = me.x;
            t.y = me.y - 1;
            deeper(t, me.top, t.bottom);
            connected = true;
        }

        return connected;
    };

    for (auto& t : tiles) {
        if (t.used || t.id == me.id) { continue; }

        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();

        t.flip_ud();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        t.flip_ud();

        t.flip_lr();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        if (check_match(t)) { continue; }
        t.rotate_cw_90();
        t.flip_lr();
    }

    return;
}

unsigned long long part_2(Tiles tiles) {

    std::set<unsigned> visited;

    {
        Tile& t = tiles.front();
        t.x     = 0;
        t.y     = 0;
        connect_tile(t, tiles, visited);
    }

    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();

    for (auto const& t : tiles) {
        min_x = std::min(min_x, t.x);
        min_y = std::min(min_y, t.y);
    }

    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();

    for (auto& t : tiles) {
        t.x -= min_x;
        t.y -= min_y;
        max_x = std::max(max_x, t.x);
        max_y = std::max(max_y, t.y);
    }

    ++max_x;
    ++max_y;
    constexpr unsigned img_width = 8;
    constexpr unsigned img_height = 8;

    Lines sea(max_y * img_height, std::string(max_x * img_width, ' '));

    unsigned waves = 0;
    for (auto const& t : tiles) {
        auto const& img = t.image;
        unsigned row = t.y * img_height;
        for (auto const& line : img) {
            unsigned col = t.x * img_width;
            for (auto const& c : line) {
                sea[row][col] = c;
                ++col;
                if (c == '#') {
                    ++waves;
                }
            }
            ++row;
        }
    }

    std::vector<std::regex> r_monster = {std::regex{"..................#."},
                                         std::regex{"#....##....##....###"},
                                         std::regex{".#..#..#..#..#..#..."}};
    unsigned monster_width             = 20;
    unsigned monster_height            = 3;

    auto find_monster = [&]() {
        unsigned monster_count = 0;
        for ( auto rit = sea.begin(); rit != std::prev(sea.end(), monster_height); ++rit )
        {
            for ( unsigned start = 0; start < rit->size() - monster_width; ++start )
            {
                if ( std::regex_match(rit->substr(start, monster_width), r_monster.at(0)) )
                {
                    if ( std::regex_match(std::next(rit, 1)->substr(start, monster_width), r_monster.at(1)) )
                    {
                        if ( std::regex_match(std::next(rit, 2)->substr(start, monster_width), r_monster.at(2)) )
                        {
                            ++monster_count;
                        }
                    }
                }
            }
        }
        return monster_count;
    };

    unsigned monster_count = 0;
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);

    flip_lr(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    flip_lr(sea);

    flip_ud(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    monster_count += find_monster();
    rotate_cw_90(sea);
    flip_ud(sea);

    return waves - (monster_count * 15);
}


int main(int argc, char* argv[]) {
    Lines ls;
    Tiles tiles;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            tiles.push_back(Tile::parse(ls));
            ls.clear();
            continue;
        }
        ls.push_back(std::move(line));
    }
    if (!ls.empty()) {
        tiles.push_back(Tile::parse(ls));
    }

    const auto r1 = part_1(tiles);
    std::cout << "1 Result=" << r1 << "\n";
    // assert(r1 == 27803643063307);

    const auto r2 = part_2(tiles);
    std::cout << "2 Result=" << r2 << "\n";
    // assert(r2 == 1644);

    return 0;
}
