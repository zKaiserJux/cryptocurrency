#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <string>

using Hash256 = std::array<std::uint8_t, 32>;

// compute SHA256 hash
bool computeHash(const std::vector<std::uint8_t>& unhashedInput, Hash256& hashedOut);
// hex representation
void PrintHex(const char* name, const std::array<std::uint8_t, 32>& input);