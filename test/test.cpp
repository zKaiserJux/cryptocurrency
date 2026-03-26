#include "test.h"
#include "../cryptography/SHA256.h"
#include "cryptography/keypair.h"
#include "core/Transaction.h"
#include "core/BlockHeader.h"
#include "core/Block.h"

#include <vector>
#include <iostream>


// SECTION TO DEFINE TEST OBJECTS FOR FUNCTION USAGE
CTransaction testTx(
    1,   // version
    0,   // nLockTime
    {
        TxInput{
            // previousTxHash
            {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
            },
            0,  // outputIndex
            {0x30, 0x45, 0x02, 0x21, 0x01, 0xAB, 0xCD, 0xEF}
        },
        TxInput{
            {
                0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
                0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
            },
            1,
            {0x51, 0x21, 0x02, 0xDE, 0xAD, 0xBE, 0xEF, 0xAC}
        }
    },
    {
        TxOutput{
            5000000000ULL,
            {0x76, 0xA9, 0x14, 0x11, 0x22, 0x33, 0x44, 0x55,
             0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD,
             0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x88, 0xAC}
        },
        TxOutput{
            1200000000ULL,
            {0x76, 0xA9, 0x14, 0x99, 0x88, 0x77, 0x66, 0x55,
             0x44, 0x33, 0x22, 0x11, 0x00, 0xFF, 0xEE, 0xDD,
             0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x88, 0xAC}
        }
    }
);

// test function for SHA256
bool testComputeHash() {
    const std::vector<std::uint8_t> testVector = {3, 6, 8};
    const std::array<std::uint8_t, 32> hashed = computeHash(testVector);
    std::cout << hashed.size() << std::endl;
    PrintHex("Hash: ", hashed);
    return true;
}

// test function for double SHA256
bool testDoubleHash() {
    const std::vector<std::uint8_t> testVector = {3, 6, 8};
    std::array<std::uint8_t, 32> hashedTwice = computeDoubleHash(testVector);
    std::cout << hashedTwice.size() << std::endl;
    PrintHex("Hash: ", hashedTwice);
    return true;
}

// function to test if a keypair can be generated
bool testKeyPair() {
    KeyPair keypair;
    if (keypair.GenerateEd25519KeyPair()) {
        PrintHex("Private Key:", keypair.m_privateKey.getKeyData());
        PrintHex("Public Key:", keypair.m_publicKey.getKeyData());
        return true;
    }
    return false;
}

/*
 *
 * TRANSACTION SECTION
 *
 */

void testPrintTransaction() {
    testTx.printTransaction();
}


bool testTxSerialization() {
    const std::vector<std::uint8_t> serializedTx = testTx.serialize();
    if (serializedTx.empty()) {
        return false;
    }
    PrintHex("Tx Hash: ", serializedTx);
    return true;
}

bool testTxDeserialization() {
    const std::vector<std::uint8_t> serializedTx = testTx.serialize();
    if (serializedTx.empty()) {
        return false;
    }
    const CTransaction tx = CTransaction::deserialize(serializedTx);
    tx.printTransaction();
    return true;
}