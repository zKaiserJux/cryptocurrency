#include <optional>
#include <iostream>

#include "BlockHeader.h"
#include "../cryptography/SHA256.h"

// template function to add different integral datatypes to bytestream (big endian)
template <typename T> void add_be(T& t, std::vector<std::uint8_t>& out) {
    // check if T is integral type
    static_assert(std::is_integral_v<T>, "add_be datatype needs to be integral type");

    using U = std::make_unsigned_t<T>;
    U u = static_cast<U>(t);

    for (int i = sizeof(U) - 1; i >= 0; --i) {
        out.push_back(static_cast<std::uint8_t>((u >> (i * 8)) & 0xFF));
    }
}

// template function to add different integral datatypes to bytestream (little endian)
template <typename T> void serializeField(T& t, std::vector<std::uint8_t>& out) {
    static_assert(std::is_integral_v<T>, "add_le datatype needs to be integral type");

    using U = std::make_unsigned_t<T>;
    U u = static_cast<U>(t);

    for (int i = 0; i < sizeof(T); i++) {
        out.push_back(static_cast<std::uint8_t>((u >> (i * 8)) & 0xFF));
    }
}

// template function to deserialize field of integral data type
template <typename T> T deserializeField(std::vector<std::uint8_t>& serializedBlockHeader, std::size_t& offset) {
    static_assert(std::is_integral_v<T>, "data type must be integral in block header");
    if ((sizeof(T) + offset) > serializedBlockHeader.size()) {
        throw std::runtime_error("data size is too large");
    }
    T t{};
    std::memcpy(&t, serializedBlockHeader.data() ++ offset, sizeof(T));
    offset += sizeof(T);
    return t;
}

// all fields despite hashes will be serialized little endian
std::vector<std::uint8_t> BlockHeader::serialize() const {
    // each entry of serializedHeader represents one byte
    std::vector<std::uint8_t> serializedHeader;

    serializeField(m_version, serializedHeader);
    for (const auto& byte : m_prevBlockHash) {
        serializeField(byte, serializedHeader);
    }
    for (const auto& byte : m_merkleRoot) {
        serializeField(byte, serializedHeader);
    }
    serializeField(m_timestamp, serializedHeader);
    serializeField(m_nonce, serializedHeader);

    return serializedHeader;
}

// deserialize the block header
BlockHeader BlockHeader::deserialize(std::vector<std::uint8_t>& serializedBlockHeader) {
    std::size_t offset = 0;
    const auto version = deserializeField<std::uint32_t>(serializedBlockHeader, offset);
    Hash256 prevBlockHash;
    std::memcpy(prevBlockHash.data(), serializedBlockHeader.data() + offset, prevBlockHash.size());
    offset += prevBlockHash.size();
    Hash256 merkleRoot;
    std::memcpy(merkleRoot.data(), serializedBlockHeader.data() + offset, merkleRoot.size());
    offset += merkleRoot.size();
    const auto timestamp = deserializeField<std::uint32_t>(serializedBlockHeader, offset);
    const auto nonce = deserializeField<std::uint32_t>(serializedBlockHeader, offset);
    const BlockHeader blockHeader(version, prevBlockHash, merkleRoot, timestamp, nonce);
    return blockHeader;
}

// create SHA256 of serialized block header
std::optional<Hash256> BlockHeader::hashBlockHeader() const {
    Hash256 hashed{};
    if (std::vector<std::uint8_t> unhashed = serialize(); !computeHash(unhashed, hashed)) {
        return std::nullopt;
    }
    return hashed;
}

// print deserialized block header
void BlockHeader::printBlockHeader() const {
    std::cout << "{ " << std::endl;
    std::cout << "  version:  " << m_version << std::endl;
    PrintHex("  prevBlockHash", m_prevBlockHash);
    PrintHex("  merkleRoot", m_merkleRoot);
    std::cout << "  timestamp:  " << m_timestamp << std::endl;
    std::cout << "  nonce:    " << m_nonce << std::endl;
    std::cout << "}" << std::endl;
}
