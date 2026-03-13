#include <optional>

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
template <typename T> void add_le(T& t, std::vector<std::uint8_t>& out) {
    static_assert(std::is_integral_v<T>, "add_le datatype needs to be integral type");

    using U = std::make_unsigned_t<T>;
    U u = static_cast<U>(t);

    for (int i = 0; i < sizeof(T); i++) {
        out.push_back(static_cast<std::uint8_t>((u >> (i * 8)) & 0xFF));
    }
}

// all fields despite hashes will be serialized little endian
std::vector<std::uint8_t> BlockHeader::serialize() const {
    // each entry of serializedHeader represents one byte
    std::vector<std::uint8_t> serializedHeader;

    add_le(m_version, serializedHeader);
    for (const auto& byte : m_prevBlockHash) {
        add_be(byte, serializedHeader);
    }
    for (const auto& byte : m_merkleRoot) {
        add_be(byte, serializedHeader);
    }
    add_le(m_timestamp, serializedHeader);
    add_le(m_nonce, serializedHeader);

    return serializedHeader;
}

// create SHA256 of serialized block header
std::optional<Hash256> BlockHeader::hashBlockHeader() const {
    Hash256 hashed{};
    if (std::vector<std::uint8_t> unhashed = serialize(); !computeHash(unhashed, hashed)) {
        return std::nullopt;
    }
    return hashed;
}