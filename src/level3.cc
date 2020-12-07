#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[]) {
    std::vector<std::string> mapa;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        mapa.push_back(line);
    }

    auto eval = [&mapa](unsigned step_x, unsigned step_y) -> unsigned {
        unsigned trees = 0;
        const auto max_x = mapa[0].size();
        unsigned x = 0;

        for (auto y = mapa.begin(); y != mapa.end();) {
            if ((*y)[x] == '#') { ++trees; }
            x = (x + step_x) % max_x;
            for (int j = 0; j < step_y; ++j) {
                std::advance(y, 1);
                if (y == mapa.end()) { break; }
            }
        }
        return trees;
    };

    auto p1 = eval(1, 1);
    auto p2 = eval(3, 1);
    auto p3 = eval(5, 1);
    auto p4 = eval(7, 1);
    auto p5 = eval(1, 2);

    std::cout << "Hit " << (p1 * p2 * p3 * p4 * p5) << " trees\n";

    return 0;
}
