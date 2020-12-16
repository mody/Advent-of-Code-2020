#include <iostream>
#include <vector>
#include <map>

using Numbers = std::vector<unsigned>;


unsigned work(Numbers const& n, unsigned last_step) {
    std::map<unsigned, std::pair<unsigned, unsigned>> data;

    unsigned step = 1;
    for (unsigned i : n) {
        data.insert({i, {0, step++}});
    }

    unsigned last = n.back();
    for ( ; step <= last_step; ++step )
    {
        // std::cout << "step=" << step << ", last=" << last;
        auto it = data.find(last);
        if (it == data.end())
        {
            // std::cout << ", not found: 0\n";
            last = 0;
        }
        else
        {
            auto& p = it->second;
            if (p.first == 0) {
                // std::cout << ", found but like not found: 0\n";
                last = 0;
            } else {
                last = p.second - p.first;
                // std::cout << ", found on " << p.first << " and " << p.second << " : " << last << "\n";
            }
        }
        auto jj = data.insert({last, {0, step}});
        if ( ! jj.second )
        {
            auto& p2  = jj.first->second;
            p2.first  = p2.second;
            p2.second = step;
        }
    }

    return last;
}


int main(int argc, char* argv[]) {
    std::cout << "1 Result = " << work({0, 12, 6, 13, 20, 1, 17}, 2020) << "\n";
    std::cout << "2 Result = " << work({0, 12, 6, 13, 20, 1, 17}, 30000000) << "\n";
    return 0;
}
