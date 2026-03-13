#pragma once

#include <array>
#include <span>
#include <vector>

using uint256 = std::array<std::uint8_t, 32>;

// compute SHA256 hash
bool computeHash(std::span<const std::uint8_t> unhashedInput, uint256& hashedOut);
// hex representation
void PrintHex(const char* name, std::span<const std::uint8_t> input);
// compute double SHA256
bool computeDoubleHash(const std::vector<std::uint8_t>& unhashedInput, uint256& hashedOut);