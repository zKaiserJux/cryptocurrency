#pragma once
#include "BlockHeader.h"
#include "Transaction.h"


#ifndef BLOCK_H
#define BLOCK_H

class Block {
    public:
    Block(const BlockHeader& header, const std::vector<CTransaction>& transactions) : m_header(header), m_transactions(transactions) {};
    ~Block() = default;

    void addTx(const CTransaction& tx);
    [[nodiscard]] std::vector<std::uint8_t> serializeBlock() const;
    [[nodiscard]] static Block deserializeBlock(const std::vector<std::uint8_t>& serializedBlock);
    [[nodiscard]] std::array<std::uint8_t, 32> calculateMerkleRoot() const;
    // update the merkle root inside the block header class after calculation
    void updateMerkleRoot(const std::array<std::uint8_t, 32>& merkleRoot);
    [[nodiscard]] static const std::array<std::uint8_t, 32> calculateBlockHash(std::vector<std::uint8_t>& serializedBlock);
    void printBlock() const;
    void setBlockID();

    private:
    BlockHeader m_header;
    std::vector<CTransaction> m_transactions;
};

#endif