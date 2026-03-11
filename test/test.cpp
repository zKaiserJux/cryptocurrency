#include "test.h"
#include "../cryptography/SHA256.h"
#include "cryptography/keypair.h"

#include <vector>
#include <iostream>

bool testComputeHash() {
    std::vector<std::uint8_t> myvec = {3, 6, 8};
    std::array<std::uint8_t, 32> hashed{};
    if (computeHash(myvec, hashed)) {
        std::cout << hashed.size() << std::endl;
        PrintHex("Hash", hashed);
        return true;
    }
    return false;
}

bool testKeyPair() {
    KeyPair keypair;
    if (keypair.GenerateEd25519KeyPair()) {
        PrintHex("Private Key:", keypair.m_privateKey.getKeyData());
        PrintHex("Public Key:", keypair.m_publicKey.getKeyData());
        return true;
    }
    return false;
}