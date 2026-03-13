#pragma once
#include <array>
#include <cstdint>
#include <vector>

// structure used for transaction inputs
struct TxInput {
    [[nodiscard]] std::vector<std::uint8_t> serialize() const;
    // hash of the transaction output
    std::array<std::uint8_t, 32> previousTxHash;
    // index of old transaction
    std::uint32_t outputIndex;
    // signing data
    std::vector<std::uint8_t> unlockingData;
};

// structure used for transaction outputs
struct TxOutput {
    [[nodiscard]] std::vector<std::uint8_t> serialize() const;
    // amount of coins spent
    std::uint64_t amount;
    // who can spend the coin later on -> digital signature
    std::vector<std::uint8_t> unlockingData;
};

#ifndef TRANSACTION_H
#define TRANSACTION_H

// wallet addresses are 32 byte integers
using uint256 = std::array<std::uint8_t, 32>;

class CTransaction {
    public:
    CTransaction() = default;
    ~CTransaction() = default;
    [[nodiscard]] const std::vector<std::uint8_t> serialize() const;
    const CTransaction deserialize() const;
    [[nodiscard]] uint256 hashTransaction(const std::vector<std::uint8_t>& serializedTx);

    private:
    std::vector<TxInput> m_txInput;
    std::vector<TxOutput> m_txOutput;
    std::uint32_t m_version;
    std::uint32_t m_nLockTime;
};

#endif