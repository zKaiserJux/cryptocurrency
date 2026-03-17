#pragma once
#include "BlockHeader.h"
#include "Transaction.h"


#ifndef BLOCK_H
#define BLOCK_H

class Block {
    public:
    Block() = delete;
    ~Block() = default;
    // function to add a serialized transaction to the block data
    void addTx(const std::vector<std::uint8_t>& tx);
    [[nodiscard]] const std::vector<std::uint8_t> serializeBlock() const;
    [[nodiscard]] const Block deserializeBlock() const;
    [[nodiscard]] std::array<std::uint8_t, 32> calculateMerkleRoot() const;

    void setMerkleRoot(const std::array<std::uint8_t, 32>& root);
    void setBlockID();

    std::size_t getBlockID() const;

    private:
    inline static std::size_t nextBlockID = 0;
    std::size_t m_blockID;
    BlockHeader m_header;
    std::vector<std::uint8_t> serializedTxs{};
    std::array<std::uint8_t, 32> m_merkleRoot;

};

#endif