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

// function to compute the SHA-256 hash algorithm
bool computeHash(const std::vector<std::uint8_t>&  unhashedInput, Hash256& hashedOut) {
    const OpenSSLPointer<EVP_MD_CTX> context(EVP_MD_CTX_new());

    if (context.get() == nullptr) {
        return false;
    }

    if (!EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr)) {
        return false;
    }

    if (!unhashedInput.empty()) {
        if (EVP_DigestUpdate(context.get(), unhashedInput.data(), unhashedInput.size()) != 1) {
            return false;
        }
    }

    unsigned int lengthOfHash = 0;

    if (!EVP_DigestFinal_ex(context.get(), hashedOut.data(), &lengthOfHash)) {
        return false;
    }

    if (lengthOfHash != hashedOut.size()) {
        return false;
    }

    return true;
}

void PrintHex(const char* name, const std::array<std::uint8_t, 32>& input)
{
    std::cout << name << " (" << input.size() << " bytes): ";
    for (const std::uint8_t b : input) {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<unsigned>(b);
    }
    std::cout << std::dec << '\n';
}