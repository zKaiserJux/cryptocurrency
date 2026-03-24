#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "SHA256.h"
#include <openssl/evp.h>

struct OpenSSLFree {
    void operator()(void* ptr) const {
        EVP_MD_CTX_free(static_cast<EVP_MD_CTX *>(ptr));
    }
};

template <typename T>
using OpenSSLPointer = std::unique_ptr<T, OpenSSLFree>;

// function to compute the SHA-256 of a container
uint256 computeHash(const std::span<const std::uint8_t>& unhashedInput) {
    const OpenSSLPointer<EVP_MD_CTX> context(EVP_MD_CTX_new());

    if (context.get() == nullptr) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }

    if (!EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr)) {
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    if (!unhashedInput.empty()) {
        if (EVP_DigestUpdate(context.get(), unhashedInput.data(), unhashedInput.size()) != 1) {
            throw std::runtime_error("EVP_DigestUpdate failed");
        }
    }

    unsigned int lengthOfHash = 0;
    uint256 hashedOut;

    if (!EVP_DigestFinal_ex(context.get(), hashedOut.data(), &lengthOfHash)) {
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    if (lengthOfHash != hashedOut.size()) {
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    return hashedOut;
}

// function to represent byte streams as hex
void PrintHex(const char* name, std::span<const std::uint8_t> input) {
    std::cout << name << ": ";
    for (const std::uint8_t byte : input) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte);
    }
    std::cout << std::dec << std::endl;
}

// function to compute the double SHA-256 used for hashing transactions
uint256 computeDoubleHash(const std::span<const std::uint8_t>& unhashedInput) {
    const uint256 hashedOnce = computeHash(unhashedInput);
    const uint256 hashedTwice = computeHash(hashedOnce);
    return hashedTwice;
}
