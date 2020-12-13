#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct Bus {
    unsigned interval = 0;
    unsigned offset = 0;
    unsigned long long departure = 0;
};

using Timetable = std::vector<Bus>;

Timetable parse_tt(std::string const& line)
{
    std::vector<std::string> v;
    boost::algorithm::split(v, line, boost::algorithm::is_any_of(","));

    Timetable result;
    unsigned offset = 0;
    for (auto const& i : v) {
        Bus t;
        t.offset = offset++;
        if (i != "x") {
           t.interval = std::stoi(i);
           result.push_back(std::move(t));
        }
    }
    return result;
}

void part_1(Timetable tt, unsigned my_time)
{
    for (auto & t : tt) {
        for (; t.departure < my_time; t.departure += t.interval)
            ;
        t.departure -= my_time;
    }

    for (auto const& t : tt) {
        std::cout << " (" << t.interval << ", " << t.offset << ", " << t.departure << "); ";
    }
    std::cout << "\n";

    unsigned lowest = std::numeric_limits<unsigned>::max();
    unsigned pos = std::numeric_limits<unsigned>::max();
    for (unsigned i = 0; i < tt.size(); ++i) {
        auto const& t = tt.at(i);
        if (t.departure < lowest) {
            lowest = t.departure;
            pos = i;
        }
    }

    std::cout << "1 Result=" << lowest * tt.at(pos).interval << "\n";
}

void part_2_thread(Timetable tt)
{
    bool stop_all = false;

    auto t = [&tt, &stop_all](int base_time, int threads) {
        auto je_bezezbytku = [](long long delenec, long long delitel) -> bool {
            return ((delenec / delitel) * delitel == delenec);
        };
        bool done = false;
        unsigned long long timestamp = base_time;
        for (;; timestamp += threads) {
            done = true;
            for (auto const& bus : tt) {
                if (!je_bezezbytku((timestamp + bus.offset), bus.interval)) {
                    done = false;
                    break;
                }
            }
            if (done || stop_all) { break; }
        }
        if (done) {
            std::cout << "2 Result=" << timestamp << "\n";
            stop_all = true;
        }
    };

    constexpr int NUM_THREADS = 15;
    std::vector<std::thread> work;
    for (unsigned i = 0; i < NUM_THREADS; ++i) {
        work.push_back(std::thread {t, i, NUM_THREADS});
    }

    for (auto& t : work) {
        t.join();
    }
}

void part_2_slow(Timetable tt) {
    auto je_bezezbytku = [](long long delenec, long long delitel) -> bool {
        return delenec%delitel == 0;
    };

    // by interval descending
    std::sort(tt.begin(), tt.end(), [](Bus const& lhs, Bus const& rhs) { return lhs.interval > rhs.interval; });

    bool done = true;
    unsigned long long timestamp = 0;
    Bus& b1 = tt.front();
    for(;;) {
        timestamp += b1.interval;

        done = true;
        for ( auto const& bus : tt )
        {
            if ( bus.interval == b1.interval )
            {
                continue;
            }
            int offset = bus.offset - b1.offset;
            if ( ! je_bezezbytku((timestamp + offset), bus.interval) )
            {
                done = false;
                break;
            }
        }
        if ( done )
        {
            break;
        }
    }
    if ( done )
    {
        std::cout << "2 Result=" << timestamp - b1.offset << "\n";
    }
}

void part_2(Timetable tt) {
    unsigned long long timestamp = 0;
    unsigned long long departure = tt.front().interval;

    for (auto next = std::next(tt.begin()); next != tt.end(); ++next) {
        Bus const& bus = *next;
        bool done = false;
        do {
            timestamp += departure;

            if ( ((timestamp + bus.offset) % bus.interval) == 0) {
                done = true;
                break;
            }
        } while(!done);

        departure *= bus.interval;
    }
    std::cout << "2 Result=" << timestamp << "\n";
}



int main(int argc, char* argv[])
{
    std::string line;

    std::getline(std::cin, line);
    unsigned my_time = std::stoi(line);

    std::getline(std::cin, line);
    Timetable tt {parse_tt(line)};

    for (auto const& t : tt) {
        std::cout << " (" << t.interval << ", " << t.offset << "); ";
    }
    std::cout << "\n";

    part_1(tt, my_time);
    part_2(tt);


    return 0;
}
