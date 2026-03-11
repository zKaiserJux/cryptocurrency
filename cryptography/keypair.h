//
// Created by Julius Eitz on 11.03.26.
//
#pragma once

#include "privkey.h"
#include "pubkey.h"

#ifndef KEYPAIR_H
#define KEYPAIR_H

/* class that holds the keypair containing the private and public key generated via the Ed25519
 * both keys are 32 bytes
 */
using keyBytes = std::array<std::uint8_t, 32>;

class KeyPair {
public:
    KeyPair() = default;
    [[nodiscard]] bool GenerateEd25519KeyPair();

    [[nodiscard]] const CPrivKey& GetPrivateKey() const noexcept { return m_privateKey; }
    [[nodiscard]] const CPubKey& GetPublicKey() const noexcept { return m_publicKey; }

// private:
    CPrivKey m_privateKey;
    CPubKey m_publicKey;
};

#endif