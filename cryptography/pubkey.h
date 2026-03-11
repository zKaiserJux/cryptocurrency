#pragma once
#include <vector>

#ifndef PUBKEY_H
#define PUBKEY_H

using uint256 = std::array<unsigned char, 32>;

class CPubKey {
    public:
        CPubKey() = default;
        // TODO: clear key data via destructor

        [[nodiscard]] uint256& getKeyData();
        [[nodiscard]] bool empty() const { return m_bytes.empty(); }
        [[nodiscard]] bool verify() const;
        int size() const;

    private:
        uint256 m_bytes = {0};
};

#endif

