Checklist for own cryptocurrency project in C++:

    1. Created narrow project skeleton 
        - core -> BlockChain, Block, BlockHeader, Transactions
        - cryptocurrency -> SHA256, keypairs (private and public key)
        - test -> directory for all the test files to test functionality of different functions
    
    2.  Implemented Serialization (can be used for various purposes)
        - TODO: Deserialization

    3. Implemented keypairs
        - generating private and public key via Ed25519
        
        TODO (later step)
        - Sign with private key
        - validate with public key

    Next Steps:
    4. Define block structure and implement logic of merkle root 
        - yet to decide which format to use for transactions (JSON?)
        - Create the blockchain with transactions and therefore merkle root

    5. Setting up the P2P network
        - setting up nodes in the networks (wallets, miners)

