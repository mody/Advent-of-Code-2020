#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <regex>
#include <string>
#include <variant>
#include <vector>
#include <set>


std::string to_bits(uint64_t val)
{
    std::string s;
    uint64_t mask = 1ULL << 63;
    for (unsigned i = 0; i < 64; ++i) {
        if (val & mask) {
            s.append("1");
        } else {
            s.append("0");
        }
        mask >>= 1;
    }
    return s;
}


struct Mask {
    Mask() = default;

    Mask(std::string const& input) : data{input} {
        mask = [&input]() {
            auto raw = input.substr(7);
            std::reverse(raw.begin(), raw.end());
            return raw;
        }();
        uint64_t bit = 1;
        for (auto const& c : mask) {
            if (c == '1') {
                mask_or |= bit;
            } else if (c == '0') {
                mask_and &= ~bit;
            } else if (c == 'X') {
                bit_pos.push_back(bit);
                mask_x &= ~bit;
            } else {
                std::cout << "ERROR! Unknown mask [" << c << "]\n";
                exit(1);
            }
            bit = bit << 1;
        }
    }

    uint64_t mask1(uint64_t val) const noexcept {
        val &= mask_and;
        val |= mask_or;
        return val;
    }

    std::set<uint64_t> mask2(uint64_t val) const noexcept {
        std::set<uint64_t> addresses;

        val &= mask_x;
        val |= mask_or;

        uint64_t max = (1ULL << bit_pos.size()) - 1;
        for (uint64_t i = 0; i <= max; ++i) {
            uint64_t new_val = val;
            uint64_t i_mask = 1;
            for (uint64_t bit : bit_pos) {
                if (i & i_mask) { // 1
                    new_val |= bit;
                } else { // 0
                    new_val &= ~bit;
                }
                i_mask <<= 1;
            }
            addresses.insert(new_val);
        }

        return addresses;
    }

    uint64_t mask_and = std::numeric_limits<uint64_t>::max(), mask_or = 0;
    uint64_t mask_x = std::numeric_limits<uint64_t>::max();
    std::string mask;
    std::vector<uint64_t> bit_pos;
    std::string data;
};

struct Store {
    std::regex MEM_RX{"^mem\\[(\\d+)] = (\\d+)"};

    Store(std::string const& input) {
        std::smatch m;
        if (!std::regex_match(input, m, MEM_RX)) {
            std::cout << "ERROR! Unknown command! [" << input << "]\n";
            exit(1);
        }
        position = std::stoull(m.str(1));
        value = std::stoull(m.str(2));
    }

    uint64_t position, value;
};

using Command = std::variant<Mask, Store>;
using Program = std::vector<Command>;
using Memory = std::map<uint64_t, uint64_t>;

void part_1(Program const& p)
{
    Mask mask;
    Memory mem;
    for (auto const& cmd : p) {
        std::visit([&mem, &mask](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Mask>) {
                mask = arg;
            } else if constexpr (std::is_same_v<T, Store>) {
                mem[arg.position] = mask.mask1(arg.value);
            }
        }, cmd);
    }

    const uint64_t sum = std::accumulate(
        mem.begin(), mem.end(), (uint64_t)0, [](uint64_t a, auto const& kv) { return a + kv.second; });

    std::cout << "1 Result=" << sum << "\n";
}

void part_2(Program const& p) {
    Memory mem;

    Mask mask;
    for (auto const& cmd : p) {
        std::visit([&mem, &mask](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Mask>) {
                mask = arg;
            } else if constexpr (std::is_same_v<T, Store>) {
                auto const& addrs = mask.mask2(arg.position);
                for (auto const& pos : addrs) {
                    mem[pos] = arg.value;
                }
            }
        }, cmd);
    }

    const uint64_t sum = std::accumulate(
        mem.begin(), mem.end(), (uint64_t)0, [](uint64_t a, auto const& kv) { return a + kv.second; });

    std::cout << "2 Result=" << std::dec << sum << "\n";
}

int main(int argc, char* argv[]) {

    Program p;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        if (line.substr(0, 4) == "mask") {
            p.push_back(Mask{line});
        } else if (line.substr(0, 3) == "mem") {
            p.push_back(Store{line});
        } else {
            std::cout << "ERROR! Invalid command! [" << line << "]\n";
            exit(1);
        }
    }

    part_1(p);

    part_2(p);

    return 0;
}
