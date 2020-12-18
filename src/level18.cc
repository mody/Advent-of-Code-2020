#include <cassert>
#include <iostream>
#include <regex>

const std::regex EXPR_RX{"(\\d+) ([+*]) (\\d+)"};
const std::regex SINGLE_NUM{"(\\((\\d+)\\))"};

const std::regex EXPR_ADD{"(\\d+) (\\+) (\\d+)"};
const std::regex EXPR_MUL_L{"(\\d+) (\\*) (\\d+) ([^+*])"};
const std::regex EXPR_MUL_E{"(\\d+) (\\*) (\\d+)([)])"};
const std::regex EXPR_MUL{"(\\d+) (\\*) (\\d+)"};

unsigned long part_1(std::string work) {
    std::smatch m;
    for ( bool changed = true; changed;)
    {
        changed = false;
        while ( std::regex_search(work, m, SINGLE_NUM) )
        {
            work.replace(m.position(), m.length(), m.str(2));
            changed = true;
        }
        if ( std::regex_search(work, m, EXPR_RX) )
        {
            unsigned long x      = std::stoul(m.str(1));
            unsigned long y      = std::stoul(m.str(3));
            unsigned long result = 0;
            if ( m.str(2) == "+" )
            {
                result = x + y;
            }
            else if ( m.str(2) == "*" )
            {
                result = x * y;
            }
            work.replace(m.position(), m.length(), std::to_string(result));
            changed = true;
        }
    }
    return std::stoull(work);
}


unsigned long part_2(std::string work) {
    std::smatch m;
    for ( bool changed = true; changed;)
    {
        changed = false;
        while ( std::regex_search(work, m, SINGLE_NUM) )
        {
            work.replace(m.position(), m.length(), m.str(2));
            changed = true;
        }
        while ( std::regex_search(work, m, EXPR_ADD) )
        {
            unsigned long x      = std::stoul(m.str(1));
            unsigned long y      = std::stoul(m.str(3));
            unsigned long result = x + y;
            work.replace(m.position(), m.length(), std::to_string(result));
            changed = true;
        }
        if (changed) {
            continue;
        }
        if ( std::regex_search(work, m, EXPR_MUL_L) || std::regex_search(work, m, EXPR_MUL_E) ||
             std::regex_search(work, m, EXPR_MUL) )
        {
            unsigned long x      = std::stoul(m.str(1));
            unsigned long y      = std::stoul(m.str(3));
            unsigned long result = x * y;
            if ( m.size() == 5 ) {
                work.replace(m.position(), m.length(), std::to_string(result) + m.str(4));
            } else {
                work.replace(m.position(), m.length(), std::to_string(result));
            }
            changed = true;
        }
    }
    return std::stoull(work);
}


int main(int argc, char* argv[]) {

    assert(part_1("2 * 3 + (4 * 5)") == 26);
    assert(part_1("1 + (2 * 3) + (4 * (5 + 6))") == 51);
    assert(part_1("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 437);
    assert(part_1("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 12240);
    assert(part_1("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 13632);

    assert(part_2("1 + 2 * 3 + 4 * 5 + 6") == 231);
    assert(part_2("1 + (2 * 3) + (4 * (5 + 6))") == 51);
    assert(part_2("2 * 3 + (4 * 5)") == 46);
    assert(part_2("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 1445);
    assert(part_2("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060);
    assert(part_2("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340);

    unsigned long long res_1 = 0, res_2 = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        res_1 += part_1(line);
        res_2 += part_2(line);
    }
    assert(res_1 == 1451467526514);
    assert(res_2 == 224973686321527);

    std::cout << "1 Result=" << res_1 << "\n";
    std::cout << "2 Result=" << res_2 << "\n";
    return 0;
}
