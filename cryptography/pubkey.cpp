#include "pubkey.h"

uint256& CPubKey::getKeyData() {
    return m_bytes;
}

int CPubKey::size() const {
    return m_bytes.size();
}