#include "Transaction.h"
#include "cryptography/SHA256.h"

#include <iostream>
#include <span>
#include <utility>

// template function used to add a field to the output bytestream
template <typename T> void addElement(T& t, std::vector<std::uint8_t>& out) {
    // function only works with integral datatypes
    static_assert(std::is_integral_v<T>, "Datatype must be integral");
    const std::size_t size = sizeof(T);
    for (std::size_t i = size - 1; i > 0; --i) {
        out.push_back(static_cast<std::uint8_t>(t >> (i * 8)) & 0xFF);
    }
};

// function to deserialize a field with the given datatype T
template <typename T> T deserializeField(const std::vector<std::uint8_t>& data, std::size_t& offset) {
    static_assert(std::is_integral_v<T>, "Datatype must be integral");
    if ((sizeof(T) + offset) > data.size()) {
        throw std::runtime_error("unexpected data size");
    }
    T t{};
    std::memcpy(&t, data.data() + offset, sizeof(T));
    offset += sizeof(T);
    return t;
}

// function to serialize inputs of transactions
std::vector<std::uint8_t> TxInput::serialize() const {
    std::vector<std::uint8_t> serializedInputTx{};
    // we do not need a size field for the hash as it is const with byte length of 32
    for (const auto& bytes : previousTxHash) {
        addElement(bytes, serializedInputTx);
    }
    addElement(outputIndex, serializedInputTx);
    // we need to add field for unlockingData size as it can vary
    std::uint32_t size = unlockingData.size();
    addElement(size, serializedInputTx);
    for (const auto& bytes : unlockingData) {
        addElement(bytes, serializedInputTx);
    }
    return serializedInputTx;
}

TxInput TxInput::deserialize(const std::vector<std::uint8_t>& serializedInput, std::size_t& offset) {
    TxInput txInput{};
    std::memcpy(&txInput.previousTxHash, serializedInput.data() + offset, txInput.previousTxHash.size());
    offset += txInput.previousTxHash.size();
    txInput.outputIndex = deserializeField<std::uint32_t>(serializedInput, offset);
    const auto size = deserializeField<std::size_t>(serializedInput, offset);
    txInput.unlockingData = std::vector<std::uint8_t>(serializedInput.data() + offset, serializedInput.data() + offset + size);
    offset += size;
    return txInput;
}

// print out deserialized transaction input
void TxInput::printTxInput() const {
    std::cout << "  { " << std::endl;
    PrintHex("      previous tx hash: ", previousTxHash);
    std::cout << "      output index: " << outputIndex << std::endl;
    PrintHex("      unlocking data: ", unlockingData);
    std::cout << "  }" << std::endl;
}

// function to serialize outputs of a transaction
std::vector<std::uint8_t> TxOutput::serialize() const {
    std::vector<std::uint8_t> serializedOutput{};
    addElement(amount, serializedOutput);
    // we need the size of the std::vector as it varies in size depending on the containing elements
    std::uint32_t size = unlockingData.size();
    addElement(size, serializedOutput);
    for (const auto& bytes : unlockingData) {
        addElement(bytes, serializedOutput);
    }
    return serializedOutput;
}

TxOutput TxOutput::deserialize(const std::vector<std::uint8_t>& serializedOutput, std::size_t& offset) {
    TxOutput txOutput{};
    txOutput.amount = deserializeField<std::uint64_t>(serializedOutput, offset);
    const auto size = deserializeField<std::uint32_t>(serializedOutput, offset);
    txOutput.unlockingData = std::vector<std::uint8_t>(serializedOutput.data() + offset, serializedOutput.data() + offset + size);
    return txOutput;
}

// print out deserialized transaction output
void TxOutput::printTxOutput() const {
    std::cout << "  {" << std::endl;
    std::cout << "      amount: " << std::endl;
    PrintHex("      unlocking data: ", unlockingData);
    std::cout << "  }" << std::endl;
}

/*
 * SECTION FOR CTransaction
 */

// constructor for CTransaction
CTransaction::CTransaction(const std::uint32_t version, const std::uint32_t nLockTime, std::vector<TxInput> txInput, std::vector<TxOutput> txOutput) : m_version(version), m_nLockTime(nLockTime), m_txInput(std::move(txInput)), m_txOutput(std::move(txOutput))
{ }

// function to serialize transactions
const std::vector<std::uint8_t> CTransaction::serialize() const {
    std::vector<std::uint8_t> serializedTx{};

    addElement(m_version, serializedTx);
    addElement(m_nLockTime, serializedTx);

    /* in order to get the deserialization right we need to know how many in- and output transactions we have
     * we need to create a field to tell us the number of in- and output transactions in front of the raw
     * transaction data -> we use a 4 byte field to indicate the number of in- and output transactions
     */
    std::uint32_t inputTxSize = m_txInput.size();
    addElement(inputTxSize, serializedTx);
    for (const auto& inputTx : m_txInput) {
        auto bytes = inputTx.serialize();
        serializedTx.insert(serializedTx.end(), bytes.begin(), bytes.end());
    }

    std::uint32_t outputTxSize = m_txOutput.size();
    addElement(outputTxSize, serializedTx);
    for (const auto& outputTx : m_txOutput) {
        auto bytes = outputTx.serialize();
        serializedTx.insert(serializedTx.end(), bytes.begin(), bytes.end());
    }

    /* as we do not know the size of the total transaction to the very end
     * we need to add it after we serialized alle the input and output transactions
     * and put it in front of the serialized byte stream for the transaction
     * in order to achieve that, we create a new bytestream and reserve the total size of the transaction
     */
    std::vector<std::uint8_t> outputSerializedTx;
    outputSerializedTx.reserve(sizeof(std::uint32_t) + serializedTx.size());
    const auto sizeTx = static_cast<std::uint32_t>(serializedTx.size());

    for (std::uint32_t i = sizeof(sizeTx) - 1; i > 0; --i) {
        outputSerializedTx.push_back(static_cast<std::uint8_t>(sizeTx >> (i * 8)) & 0xFF);
    }
    outputSerializedTx.insert(outputSerializedTx.end(), serializedTx.begin(), serializedTx.end());
    return outputSerializedTx;
}

// function to deserialize a transaction
CTransaction CTransaction::deserialize(const std::vector<uint8_t> &serializedTx) {
    std::size_t offset = 0;
    const auto version = deserializeField<std::uint32_t>(serializedTx, offset);
    const auto nLockTime = deserializeField<std::uint32_t>(serializedTx, offset);
    const auto inputCount = deserializeField<std::uint32_t>(serializedTx, offset);
    std::vector<TxInput> txInput{};
    txInput.reserve(inputCount);
    for (std::size_t i = 0; i < inputCount; i++) {
        txInput.push_back(TxInput::deserialize(serializedTx, offset));
    }
    const auto outputCount = deserializeField<std::uint32_t>(serializedTx, offset);
    std::vector<TxOutput> txOutput{};
    txOutput.reserve(outputCount);
    for (std::size_t i = 0; i < outputCount; i++) {
        txOutput.push_back(TxOutput::deserialize(serializedTx, offset));
    }
    CTransaction tx{version, nLockTime, txInput, txOutput};
    return tx;
}

// returns the double SHA256 transaction
uint256 CTransaction::hashTransaction(const std::vector<std::uint8_t>& serializedTx) {
    const uint256 doubleHashed = computeDoubleHash(serializedTx);
    return doubleHashed;
}

// print out deserialized transaction
void CTransaction::printTransaction() const {
    std::cout << "{" << std::endl;
    std::cout << "  version: " << m_version <<  std::endl;
    std::cout << "  nLockTime: " << m_nLockTime << std::endl;
    for (const auto& txInput : m_txInput) {
        txInput.printTxInput();
    }
    for (const auto& txOutput : m_txOutput) {
        txOutput.printTxOutput();
    }
}