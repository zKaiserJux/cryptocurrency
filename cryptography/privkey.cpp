#include "privkey.h"

uint256& CPrivKey::getKeyData() {
    return m_privKey;
}

int CPrivKey::size() const {
    return m_privKey.size();
}