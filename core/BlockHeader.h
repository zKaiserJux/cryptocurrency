#pragma once
#include <cstdint>
#include <array>
#include <vector>

#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

// SHA256 is a 32 byte hash -> here stored byte per byte in std::array object
using Hash256 = std::array<std::uint8_t, 32>;

// class that defines the header of each block
class BlockHeader {
    public:
        BlockHeader(const std::uint32_t& blockVersion, const Hash256& prevBlockHash, const Hash256& merkleRootHash, const std::uint32_t& time, const std::uint32_t& nonce)
        : m_version(blockVersion), m_prevBlockHash(prevBlockHash), m_merkleRoot(merkleRootHash), m_timestamp(time), m_nonce(nonce) {}
        ~BlockHeader() = default;

        // needed for later deserialization inside the block class
        static constexpr std::size_t BLOCK_HEADER_SIZE = 76;

        // serialize header
        [[nodiscard]] std::vector<std::uint8_t> serialize() const;
        [[nodiscard]] static BlockHeader deserialize(std::vector<std::uint8_t>& serializedBlockHeader);
        [[nodiscard]] std::optional<Hash256> hashBlockHeader() const;

        void printBlockHeader() const;
    
    private:
        // m_version is 4 byte field -> version 2 of blocks
        const std::uint32_t m_version = 2;
        // both are single or double SHA256 hashes -> 32 bytes
        Hash256 m_prevBlockHash{};
        Hash256 m_merkleRoot{};
        // m_timestamp and m_nonce are both 4 byte fields
        std::uint32_t m_timestamp;
        std::uint32_t m_nonce;
};

#endif