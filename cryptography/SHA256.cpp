#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "SHA256.h"
#include <openssl/evp.h>

struct OpenSSLFree {
    void operator()(void* ptr) const {
        EVP_MD_CTX_free((EVP_MD_CTX*)ptr);
    }
};

template <typename T>
using OpenSSLPointer = std::unique_ptr<T, OpenSSLFree>;

// function to compute the SHA-256 hash algorithm
bool computeHash(const std::vector<std::uint8_t>& unhashed, Hash256& hashed) {
    OpenSSLPointer<EVP_MD_CTX> context(EVP_MD_CTX_new());

    if (context.get() == nullptr) {
        return false;
    }

    if (!EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr)) {
        return false;
    }

    if (!unhashed.empty()) {
        if (EVP_DigestUpdate(context.get(), unhashed.data(), unhashed.size()) != 1) {
            return false;
        }
    }

    unsigned int lengthOfHash = 0;

    if (!EVP_DigestFinal_ex(context.get(), hashed.data(), &lengthOfHash)) {
        return false;
    }

    if (lengthOfHash != hashed.size()) {
        return false;
    }

    return true;
}

std::string toHex(const Hash256& h) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::uint8_t b : h) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}