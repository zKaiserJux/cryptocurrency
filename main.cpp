#include <iostream>

#include "test/test.h"

bool checkNativeEndianness(){
    ushort a=0x1234;
    if (*reinterpret_cast<unsigned char *>(&a)==0x12) {
        std::cout << "BIG Endian" << std::endl;
        return true;
    }
    std::cout << "LITTLE Endian" << std::endl;
    return false;
}


int main() {
    // testComputeHash();
    // testDoubleHash();
    // testKeyPair();
    // checkNativeEndianness();
    // testPrintTransaction();
    testTxSerialization();
    return 0;
}