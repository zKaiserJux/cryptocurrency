#pragma once

// declaring all the test functions for different modules
bool testComputeHash();
bool testDoubleHash();
bool testKeyPair();

// TRANSACTION SECTION
void testPrintTransaction();
bool testTxSerialization();
bool testTxDeserialization();
bool testTxHashing();


// BLOCK HEADER SECTION
bool testBlockHeaderSerialization();
bool testBlockHeaderDeserialization();
bool testBlockHeaderHash();


// BLOCK SECTION
bool testBlockSerialization();
bool testBlockDeserialization();
bool testBlockHash();