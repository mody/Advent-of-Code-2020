#include <iostream>
#include <regex>
#include <string>
#include <vector>

static const std::regex RULE_RX("(\\d+)-(\\d+) (.): (.*)");

int main(int argc, char* argv[]) {
    int valid_count = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::smatch m;
        if (std::regex_match(line, m, RULE_RX) == false) {
            std::cerr << "Failed to parse [" << line << "]\n";
            exit(1);
        }

        const int pos1 = std::stoi(m.str(1)) - 1;
        const int pos2 = std::stoi(m.str(2)) - 1;
        const char znak = m.str(3)[0];
        const std::string pass{m.str(4)};

        if ((pass[pos1] == znak && pass[pos2] != znak)
            || (pass[pos1] != znak && pass[pos2] == znak)) {
            ++valid_count;
        }
    }

    std::cout << "valid_count=" << valid_count << "\n";

    return 0;
}
