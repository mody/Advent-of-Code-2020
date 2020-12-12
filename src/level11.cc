#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <string>
#include <vector>

using Mapa = std::vector<std::string>;


size_t hash(Mapa const& m) noexcept
{
    size_t seed = 0;
    for (auto const& row : m) {
        boost::hash_combine(seed, row);
    }
    return seed;
}


unsigned count_seats(Mapa const& m, char c) {
    unsigned cnt = 0;
    for (auto const& row : m) {
        for (auto const& s : row) {
            if (s == c) {
                ++cnt;
            }
        }
    }
    return cnt;
}


void dump(Mapa const& m) {
    for (auto const& row : m) {
        std::cout << row << "\n";
    }
    std::cout << "\n";
}


Mapa step_1(Mapa const& m)
{
    const unsigned rows = m.size();
    const unsigned cols = m.front().size();

    auto around = [&m, &rows, &cols](unsigned const row, unsigned const col) -> unsigned {
        std::map<unsigned char, unsigned> counts;

        if (row > 0 && col > 0 && row < (rows - 1) && col < (cols - 1)) {
            counts[m.at(row - 1).at(col - 1)] += 1;
            counts[m.at(row - 1).at(col)] += 1;
            counts[m.at(row - 1).at(col + 1)] += 1;
            counts[m.at(row + 1).at(col - 1)] += 1;
            counts[m.at(row + 1).at(col)] += 1;
            counts[m.at(row + 1).at(col + 1)] += 1;
            counts[m.at(row).at(col - 1)] += 1;
            counts[m.at(row).at(col + 1)] += 1;
        } else if (row == 0) {
            if (col == 0) {
                counts[m.at(row + 1).at(col)] += 1;
                counts[m.at(row + 1).at(col + 1)] += 1;
                counts[m.at(row).at(col + 1)] += 1;
            } else if (col == (cols - 1)) {
                counts[m.at(row + 1).at(col - 1)] += 1;
                counts[m.at(row + 1).at(col)] += 1;
                counts[m.at(row).at(col - 1)] += 1;
            } else {
                counts[m.at(row + 1).at(col - 1)] += 1;
                counts[m.at(row + 1).at(col)] += 1;
                counts[m.at(row + 1).at(col + 1)] += 1;
                counts[m.at(row).at(col - 1)] += 1;
                counts[m.at(row).at(col + 1)] += 1;
            }
        } else if (row == (rows - 1)) {
            if (col == 0) {
                counts[m.at(row - 1).at(col)] += 1;
                counts[m.at(row - 1).at(col + 1)] += 1;
                counts[m.at(row).at(col + 1)] += 1;
            } else if (col == (cols - 1)) {
                counts[m.at(row - 1).at(col - 1)] += 1;
                counts[m.at(row - 1).at(col)] += 1;
                counts[m.at(row).at(col - 1)] += 1;
            } else {
                counts[m.at(row - 1).at(col - 1)] += 1;
                counts[m.at(row - 1).at(col)] += 1;
                counts[m.at(row - 1).at(col + 1)] += 1;
                counts[m.at(row).at(col - 1)] += 1;
                counts[m.at(row).at(col + 1)] += 1;
            }
        } else if (col == 0) {
            counts[m.at(row - 1).at(col)] += 1;
            counts[m.at(row - 1).at(col + 1)] += 1;
            counts[m.at(row + 1).at(col)] += 1;
            counts[m.at(row + 1).at(col + 1)] += 1;
            counts[m.at(row).at(col + 1)] += 1;
        } else if (col == (cols - 1)) {
            counts[m.at(row - 1).at(col - 1)] += 1;
            counts[m.at(row - 1).at(col)] += 1;
            counts[m.at(row + 1).at(col - 1)] += 1;
            counts[m.at(row + 1).at(col)] += 1;
            counts[m.at(row).at(col - 1)] += 1;
        } else {
            std::cerr << "ERROR! row=" << row << ", col=" << col << "\n";
            return 0;
        }

        return counts['#'];
    };

    Mapa result = m;
    for (unsigned row = 0; row < rows; ++row) {
        for (unsigned col = 0; col < cols; ++col) {
            const char c = m.at(row).at(col);
            if (c == '.') { continue; }

            const unsigned cnt = around(row, col);
            if (c == 'L' && cnt == 0) {
                result[row][col] = '#';
            } else if (c == '#' && cnt >= 4) {
                result[row][col] = 'L';
            }
        }
    }
    return result;
}

Mapa step_2(Mapa const& m)
{
    const unsigned rows = m.size();
    const unsigned cols = m.front().size();

    auto ray = [&m, &rows, &cols](int row, int col, int const dir_x, int const dir_y) -> unsigned {
        std::map<unsigned char, unsigned> counts;

        for(;;) {
            row += dir_y;
            col += dir_x;

            if (row < 0 || row == rows || col < 0 || col == cols) {
                break;
            }
            const auto c = m.at(row).at(col);
            if (c == '#' || c == 'L') {
                counts[m.at(row).at(col)] += 1;
                break;
            }
        }

        return counts['#'];
    };

    Mapa result = m;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            const char c = m.at(row).at(col);
            if (c == '.') { continue; }

            const unsigned cnt =
                ray(row, col,  0, +1) +
                ray(row, col,  0, -1) +
                ray(row, col, +1,  0) +
                ray(row, col, -1,  0) +
                ray(row, col, +1, +1) +
                ray(row, col, +1, -1) +
                ray(row, col, -1, +1) +
                ray(row, col, -1, -1);

            if (c == 'L' && cnt == 0) {
                result[row][col] = '#';
            } else if (c == '#' && cnt >= 5) {
                result[row][col] = 'L';
            }
        }
    }
    return result;
}



void part_1(Mapa m1) {
    std::set<size_t> h;
    for (;;) {
        if (!h.insert(hash(m1)).second) { break; }
        m1 = step_1(m1);
    }

    std::cout << "Iterations=" << h.size() << ", occupied=" << count_seats(m1, '#') << "\n";
}


void part_2(Mapa m1) {
    std::set<size_t> h;
    for (;;) {
        if (!h.insert(hash(m1)).second) { break; }
        m1 = step_2(m1);
    }

    std::cout << "Iterations=" << h.size() << ", occupied=" << count_seats(m1, '#') << "\n";

}

int main(int argc, char* argv[]) {
    Mapa m1;

    std::string line;
    while(std::getline(std::cin, line)) {
        m1.push_back(line);
    }

    part_1(m1);

    part_2(m1);

    return 0;
}
