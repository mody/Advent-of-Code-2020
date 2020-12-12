#include <boost/container_hash/hash.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>

using Counts = std::vector<unsigned char>;
using CountHashes = std::set<size_t>;

size_t hash(Counts const& c) noexcept
{
    size_t seed = 0;
    for (auto const& x : c) {
        boost::hash_combine(seed, x);
    }
    return seed;
}

void day10_1(std::vector<unsigned char> const& arr)
{
    std::map<unsigned char, unsigned> counts;

    unsigned char last = 0;
    for (const auto& i : arr) {
        unsigned char diff = i - last;
        counts[diff] += 1;
        if (diff > 3) {
            std::cout << "Diff exceeded! " << diff << ", last=" << last << ", i=" << i << "\n";
        }
        last = i;
    }
    counts[3] += 1;  // for our device

    std::cout << "Result=" << (counts[3] * counts[1]) << "\n";
}

unsigned count_range(Counts& haystack, Counts const& needle)
{
    assert(needle.size() < haystack.size());
    const auto len = needle.size();
    Counts::iterator from = haystack.begin(), to = std::next(from, len),
                     end = std::prev(haystack.end(), len - 1);

    unsigned count = 0;

    for (; from != end;) {
        if (needle == Counts {from, to}) {
            ++count;

            std::fill(from, to, 0);
        }

        std::advance(from, 1);
        std::advance(to, 1);
    }
    return count;
}

void day10_2(std::vector<unsigned char> const& arr)
{
    auto get_counts = [](std::vector<unsigned char> const& arr) {
        Counts counts;
        unsigned char last = 0;
        for (const auto& i : arr) {
            unsigned char diff = i - last;
            counts.push_back(diff);
            last = i;
        }
        return counts;
    };

    Counts c = get_counts(arr);

    unsigned long long result = 1;

    // modifies the haystack!
    auto find_and_pow = [](unsigned power, Counts& haystack,
                           Counts const& needle) -> unsigned long long {
        const unsigned x = count_range(haystack, needle);
        if (x) {
            unsigned long long r = power;
            for (unsigned i = 1; i < x; ++i) {
                r *= power;
            }
            return r;
        }
        return 1;
    };

    result *= find_and_pow(7, c, {1, 1, 1, 1});
    result *= find_and_pow(4, c, {1, 1, 1});
    result *= find_and_pow(2, c, {1, 1});

    std::cout << "Result=" << result << "\n";
}

int main(int argc, char* argv[])
{
    std::vector<unsigned char> arr;

    for (;;) {
        unsigned num = 0;
        std::cin >> num;
        assert(num < 256);
        if (std::cin.eof()) { break; }
        arr.push_back((unsigned char)(num));
    }

    std::sort(arr.begin(), arr.end());

    day10_1(arr);

    day10_2(arr);

    return 0;
}
