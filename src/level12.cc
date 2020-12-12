#include <iostream>
#include <map>
#include <string>
#include <vector>

using Instructions = std::vector<std::string>;

struct Dir
{
    int dx = 0;
    int dy = 0;
};

const std::map<unsigned, unsigned char> COMPASS = {{0, 'E'}, {90, 'S'}, {180, 'W'}, {270, 'N'}};

const std::map<unsigned char, Dir> DIRECTIONS = {
    {'W', {-1, 0}}, {'N', {0, -1}}, {'E', {+1, 0}}, {'S', {0, +1}}};


struct Waypoint {
    // Initial: 10 units east and 1 unit north
    int x = 10;
    int y = -1;

    /* positive = "R", negative = "L" */
    void rotate(int angle)
    {
        switch(angle) {
        case 0:
        case 360:
        case -360:
            break;
        case 270:
        case -90:
            std::swap(x, y);
            y *= -1;
            break;
        case -270:
        case 90:
            std::swap(x, y);
            x *= -1;
            break;
        case 180:
        case -180:
            x *= -1;
            y *= -1;
            break;
        default:
            std::cerr << "Unknown angle! " << angle << std::endl;
            exit(1);
        }
    }

    void dump() const
    {
        std::cout << "WAYP: x=" << x << ", y=" << y << "\n";
    }
};


struct Ship
{
    int direction = 0;
    int x = 0;
    int y = 0;

    void dump() const
    {
        std::cout << "SHIP: x=" << x << ", y=" << y << ", dir=" << direction << ", "
                  << COMPASS.at(direction) << "\n";
    }

    Ship& operator+=(Waypoint const& w) noexcept
    {
        x += w.x;
        y += w.y;
        return *this;
    };
};


void part_1(Instructions const& insrt)
{
    Ship ship;

    for (auto const& line : insrt) {
        unsigned char cmd = line.at(0);
        int param = stoi(line.substr(1));

        switch (cmd) {
        case 'N':
        case 'S':
        case 'W':
        case 'E': {
            Dir const& d = DIRECTIONS.at(cmd);
            ship.x += d.dx * param;
            ship.y += d.dy * param;
            break;
        }
        case 'F': {
            Dir const& d = DIRECTIONS.at(COMPASS.at(ship.direction));
            ship.x += d.dx * param;
            ship.y += d.dy * param;
            break;
        }
        case 'L': {
            ship.direction = std::abs(360 + ship.direction - param) % 360;
            break;
        }
        case 'R': {
            ship.direction = (ship.direction + param) % 360;
            break;
        }
        }
    }
    std::cout << "1: Manhattan=" << std::abs(ship.x) + std::abs(ship.y) << "\n";
}


void part_2(Instructions const& insrt)
{
    Ship ship;
    Waypoint waypoint;

    for (auto const& line : insrt) {
        unsigned char cmd = line.at(0);
        int param = stoi(line.substr(1));

        switch (cmd) {
        case 'N':
        case 'S':
        case 'W':
        case 'E': {
            Dir const& d = DIRECTIONS.at(cmd);
            waypoint.x += d.dx * param;
            waypoint.y += d.dy * param;
            break;
        }
        case 'F': {
            for (unsigned i=0; i < param; ++i) {
                ship += waypoint;
            }
            break;
        }
        case 'L': {
            waypoint.rotate(-param);
            break;
        }
        case 'R': {
            waypoint.rotate(param);
            break;
        }
        }
    }
    std::cout << "2: Manhattan=" << std::abs(ship.x) + std::abs(ship.y) << "\n";
}

int main(int argc, char* argv[])
{
    Instructions instr;

    std::string line;
    while (std::getline(std::cin, line)) {
        instr.push_back(line);
    }

    part_1(instr);
    part_2(instr);

    return 0;
}
