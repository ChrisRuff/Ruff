#pragma once
#include <string>
#include <array>

#include <openssl/aes.h>

#define AES_KEYLENGTH 256
namespace ruff::security
{
class AES
{
private:
    std::array<uint8_t, 32> key{};
    std::array<uint8_t, AES_BLOCK_SIZE> IV{};
public:
    AES();
    AES(const std::array<uint8_t, 32>&, const std::array<uint8_t, AES_BLOCK_SIZE>&);

    std::string Encrypt(std::string text);
    std::string Decrypt(std::string ciphertext);

    std::array<uint8_t, 32> GetKey() const { return key; };
    std::array<uint8_t, AES_BLOCK_SIZE> GetIV() const { return IV; };
};
}