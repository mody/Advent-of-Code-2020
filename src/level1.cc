#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<unsigned> arr;

    for (;;) {
        unsigned num = 0;
        std::cin >> num;
        if (std::cin.eof()) { break; }
        arr.push_back(std::move(num));
    }

    for (auto i1 = arr.begin(); i1 != std::prev(std::prev(arr.end())); ++i1) {
        for (auto i2 = std::next(i1); i2 != std::prev(arr.end()); ++i2) {
            for (auto i3 = std::next(i2); i3 != arr.end(); ++i3) {
                if ((*i1 + *i2 + *i3) == 2020) {
                    std::cout << (*i1 * *i2 * *i3) << "\n";
                    break;
                }
            }
        }
    }

    return 0;
}
