#include <cassert>
#include <iostream>
#include <limits>

struct Crypto {
    unsigned loops = 0;
};

uint64_t transform(uint64_t val, uint64_t subj) noexcept {
    return (val * subj) % 20201227;
}

Crypto find_crypto(uint64_t pubkey1) noexcept {
    uint64_t val = 1;
    for (unsigned loops = 1; loops < std::numeric_limits<unsigned>::max(); ++loops) {
        val = transform(val, 7);
        if (val == pubkey1) { return {loops}; }
    }
    assert(false);
    return {};
}

uint64_t compute_secret(uint64_t pubkey, unsigned loops) noexcept {
    uint64_t val = 1;
    for (unsigned i=0; i < loops; ++i) {
        val = transform(val, pubkey);
    }
    return val;
}


uint64_t part_1(uint64_t pubkey1, uint64_t pubkey2)
{
    Crypto c1 = find_crypto(pubkey1);
    Crypto c2 = find_crypto(pubkey2);

    auto enc1 = compute_secret(pubkey1, c2.loops);
    auto enc2 = compute_secret(pubkey2, c1.loops);
    assert(enc1 == enc2);

    return enc1;
}

int main()
{
    assert(part_1(5764801, 17807724) == 14897079);

    std::cout << "1 Result=" << part_1(10441485, 1004920) << "\n";

    return 0;
}
