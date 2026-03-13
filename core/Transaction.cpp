#include "Transaction.h"
#include "cryptography/SHA256.h"

// template function used to add a field to the output bytestream
template <typename T> void addElement(T& t, std::vector<std::uint8_t>& out) {
    // function only works with integral datatypes
    static_assert(std::is_integral_v<T>, "Datatype must be integral");
    const std::size_t size = sizeof(T);
    for (std::size_t i = size - 1; i > 0; --i) {
        out.push_back(static_cast<std::uint8_t>(t >> (i * 8)) & 0xFF);
    }
};

// function to serialize inputs of transactions
std::vector<std::uint8_t> TxInput::serialize() const {
    std::vector<std::uint8_t> serializedInputTx{};
    for (const auto& bytes : previousTxHash) {
        addElement(bytes, serializedInputTx);
    }
    addElement(outputIndex, serializedInputTx);
    for (const auto& bytes : unlockingData) {
        addElement(bytes, serializedInputTx);
    }
    return serializedInputTx;
}

// function to serialize outputs of a transaction
std::vector<std::uint8_t> TxOutput::serialize() const {
    std::vector<std::uint8_t> serializedOutput{};
    addElement(amount, serializedOutput);
    for (const auto& bytes : unlockingData) {
        addElement(bytes, serializedOutput);
    }
    return serializedOutput;
}

// function to serialize transactions
const std::vector<std::uint8_t> CTransaction::serialize() const {
    std::vector<std::uint8_t> serializedTx{};

    for (const auto& inputTx : m_txInput) {
        auto bytes = inputTx.serialize();
        serializedTx.insert(serializedTx.end(), bytes.begin(), bytes.end());
    }

    for (const auto& outputTx : m_txOutput) {
        auto bytes = outputTx.serialize();
        serializedTx.insert(serializedTx.end(), bytes.begin(), bytes.end());
    }

    addElement(m_version, serializedTx);
    addElement(m_nLockTime, serializedTx);
    return serializedTx;
}

uint256 CTransaction::hashTransaction(const std::vector<std::uint8_t>& serializedTx) {
    return {};
}
