#include <array>
#include <cstdint>


#ifndef KEY_H
#define KEY_H

class CPubKey;

// uint256 is an array that contains 1 byte per entry and represents a 32 byte unsingned number in total
using uint256 = std::array<std::uint8_t, 32>;

class CPrivKey {
    public:
        CPrivKey() = default;
        static CPrivKey generatePrivKey();
        bool isValid() const;

        CPubKey derivePubKey() const;
        const uint256& bytes() const { return m_privKey; }
        bool sign() const;

    private:
        uint256 m_privKey;
        bool m_valid{false};
};

#endif
