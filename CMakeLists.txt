cmake_minimum_required(VERSION 3.16)

project(BitcoinNode LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

add_executable(BitcoinNode
    main.cpp
    core/Block.cpp
    core/BlockHeader.cpp
    core/Transaction.cpp
    cryptography/SHA256.cpp
    cryptography/privkey.cpp
    cryptography/pubkey.cpp
    test/test.cpp
        cryptography/pubkey.h
        cryptography/keypair.cpp
        cryptography/keypair.h
        core/Transaction.h
        core/Block.h
)

target_include_directories(BitcoinNode PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BitcoinNode PRIVATE OpenSSL::Crypto)