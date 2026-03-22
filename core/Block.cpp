#include "Block.h"
#include "cryptography/SHA256.h"

template <typename T> T deserializeField(const std::vector<std::uint8_t>& data, std::uint32_t& offset) {
    static_assert(std::is_integral_v<T>, "Datatype must be integral");
    if ((sizeof(T) + offset) > data.size()) {
        throw std::runtime_error("unexpected data size");
    }
    T t{};
    std::memcpy(&t, data.data() + offset, sizeof(T));
    offset += sizeof(T);
    return t;
}


// function to add transactions to the block body
void Block::addTx(const CTransaction& tx) {
    m_transactions.push_back(tx);
}


// serialized block header in two stages: 1. serialize block header and 2. serialize all the block transactions
std::vector<std::uint8_t> Block::serializeBlock() const {
    std::vector<std::uint8_t> serializedBlock;
    // insert the serialized block header first
    serializedBlock.insert(serializedBlock.begin(), m_header.serialize().begin(), m_header.serialize().end());
    const auto txCount = static_cast<std::uint32_t>(m_transactions.size());
    for (std::uint32_t i = sizeof(txCount) - 1; i > 0; --i) {
        serializedBlock.push_back(static_cast<std::uint8_t>(txCount >> (i * 8)) & 0xFF);
    }
    for (const auto& tx : m_transactions) {
        serializedBlock.insert(serializedBlock.end(), tx.serialize().begin(), tx.serialize().end());
    }
    return serializedBlock;
}

// deserialize the block in two stages: 1. deserialize block header and 2. deserialize all the block transactions
Block Block::deserializeBlock(const std::vector<std::uint8_t>& serializedBlock) {
    std::vector serializedHeader(serializedBlock.begin(), serializedBlock.begin() + BlockHeader::BLOCK_HEADER_SIZE);
    const BlockHeader header = BlockHeader::deserialize(serializedHeader);

    // we need to increase the offset by the size of the block header to reach the transactions section
    std::uint32_t offset = serializedHeader.size();
    // vector object that stores all the deserialized transactions
    std::vector<CTransaction> transactions;
    // the first 4 bytes hold the transaction count that we need to extract from the bytestream for the loop
    const auto txCount = deserializeField<std::uint32_t>(serializedBlock, offset);
    // iterating over all transactions and adjust the offset according to different tx sizes
    for (std::uint32_t i = 0; i < (txCount - 1); i++) {
        const auto txSize = deserializeField<std::uint32_t>(serializedBlock, offset);
        std::vector serializedTx(serializedBlock.begin() + offset, serializedBlock.begin() + offset + txSize);
        CTransaction tx = CTransaction::deserialize(serializedTx);
        transactions.push_back(tx);
    }
    Block block(header, transactions);
    return block;
}

// updates the merkle root inside the block header class
void Block::updateMerkleRoot(const std::array<std::uint8_t, 32> &merkleRoot) {
    m_header.setMerkleRoot(merkleRoot);
}

// calculates the block hash of the current block
const std::array<std::uint8_t, 32> Block::calculateBlockHash(std::vector<std::uint8_t>& serializedBlock) {
    std::array<std::uint8_t, 32> blockHash{};
    if (!computeHash(serializedBlock, blockHash)) {
        throw std::runtime_error("unexpected block hash");
    }
    return blockHash;
}
