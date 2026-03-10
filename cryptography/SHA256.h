#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <string>

using Hash256 = std::array<std::uint8_t, 32>;

// compute SHA256 hash
bool computeHash(const std::vector<std::uint8_t>& unhashedInput, Hash256& out);
// hex representation
std::string toHex(const Hash256& h);