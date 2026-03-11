//
// Created by Julius Eitz on 11.03.26.
//
#include "keypair.h"

#include <iostream>
#include <openssl/evp.h>

#include <memory>
#include <ostream>

bool KeyPair::GenerateEd25519KeyPair() {
    using EvpPkeyCtxPtr = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>;
    using EvpPkeyPtr    = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;

    const EvpPkeyCtxPtr ctx(EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr), &EVP_PKEY_CTX_free);
    if (!ctx)
    {
        std::cerr << "EVP_PKEY_CTX_new_id() failed" << std::endl;
        return false;
    }

    if (EVP_PKEY_keygen_init(ctx.get()) != 1)
    {
        std::cerr << "EVP_PKEY_keygen_init() failed" << std::endl;
        return false;
    }

    EVP_PKEY* rawKey = nullptr;
    if (EVP_PKEY_keygen(ctx.get(), &rawKey) != 1)
    {
        std::cerr << "EVP_PKEY_keygen_init() failed" << std::endl;
        return false;
    }
    // rawKey points at the address of the EVP_PKEY object that contains the private and public key
    // using smart pointer to automatically free the memory if the pointer is no longer needed
    const EvpPkeyPtr pkey(rawKey, &EVP_PKEY_free);

    size_t privKeyLen = m_privateKey.size();
    if (EVP_PKEY_get_raw_private_key(
            pkey.get(),
            m_privateKey.getKeyData().data(),
            &privKeyLen) != 1)
    {
        std::cerr << "EVP_PKEY_get_raw_private_key() failed" << std::endl;
        return false;
    }

    if (privKeyLen != m_privateKey.size())
    {
        std::cerr << "EVP_PKEY_get_raw_private_key() failed" << std::endl;
        return false;
    }

    size_t pubKeyLen = m_publicKey.size();
    if (EVP_PKEY_get_raw_public_key(
            pkey.get(),
            m_publicKey.getKeyData().data(),
            &pubKeyLen) != 1)
    {
        std::cerr << "EVP_PKEY_get_raw_public_key() failed" << std::endl;
        return false;
    }

    if (pubKeyLen != m_publicKey.size())
    {
        std::cerr << "EVP_PKEY_get_raw_public_key() failed" << std::endl;
        return false;
    }

    return true;
}
