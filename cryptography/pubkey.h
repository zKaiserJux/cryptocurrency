#include <vector>
#include <cstdint>

#ifndef PUBKEY_H
#define PUBKEY_H

class CPubKey {
    public:
        CPubKey() = default;
        explicit CPubKey(std::vector<std::uint8_t> bytes);

        const std::vector<std::uint8_t>& bytes() const { return m_bytes; }
        bool empty() const { return m_bytes.empty(); }

    private:
        std::vector<std::uint8_t> m_bytes;
};

#endif

