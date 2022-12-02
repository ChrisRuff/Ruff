#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace ruff::security
{
std::string SubstitutionEncrypt(std::string msg, const std::unordered_map<char, char>& key);
std::string SubstitutionDecrypt(std::string cipher, const std::unordered_map<char, char>& key);

std::string TranspositionEncrypt(std::string msg, size_t ncols, std::vector<size_t> key);
std::string TranspositionDecrypt(std::string cipher, size_t ncols, std::vector<size_t> key);
}// namespace ruff::security