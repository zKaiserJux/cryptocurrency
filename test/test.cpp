#include "test.h"
#include "../cryptography/SHA256.h"

#include <vector>
#include <iostream>

bool testComputeHash() {
    std::vector<std::uint8_t> myvec = {3, 6, 8};
    std::array<std::uint8_t, 32> hashed;
    if (computeHash(myvec, hashed)) {
        std::cout << hashed.size() << std::endl;
        std::cout << toHex(hashed);
        return true;
    }
    return false;
}