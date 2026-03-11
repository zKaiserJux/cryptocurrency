#pragma once
#include <array>
#include <cstdint>
#include <openssl/types.h>

#ifndef KEY_H
#define KEY_H

class CPubKey;
class KeyPair;

// uint256 is an array that contains 1 byte per entry and represents a 32 byte unsigned number in total
using uint256 = std::array<std::uint8_t, 32>;

class CPrivKey {
    public:
        CPrivKey() = default;
        bool isValid() const;

        [[nodiscard]] uint256& getKeyData();
        bool sign() const;
        int size() const;

    private:
        // m_privKey is set by member function setPrivKey
        uint256 m_privKey = {0};
};
#endif
