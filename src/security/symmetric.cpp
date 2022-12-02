#include <ruff/security/symmetric.hpp>

#include <openssl/rand.h>

namespace ruff::security
{
    AES::AES()
    {
        RAND_bytes(key.data(), key.size());
        RAND_bytes(IV.data(), IV.size());
    }

    AES::AES(const std::array<uint8_t, 32>& key, const std::array<uint8_t, AES_BLOCK_SIZE>& iv) : key(key), IV(iv) {}

    std::string AES::Encrypt(std::string text)
    {
        AES_KEY* encrypt_key = new AES_KEY();
        AES_set_encrypt_key(key.data(), AES_KEYLENGTH, encrypt_key);

        std::string padded_text = text;
        size_t padding = (AES_BLOCK_SIZE - (text.length() % AES_BLOCK_SIZE));
        for(size_t i = 0; i < padding; ++i)
            padded_text.push_back(0);

        unsigned char* aes_input = reinterpret_cast<unsigned char*>(padded_text.data());
        unsigned char ciphertext[padded_text.size()] = {0};

        std::array<uint8_t, AES_BLOCK_SIZE> tmp_iv = IV;
        AES_cbc_encrypt(aes_input, ciphertext, padded_text.length(),
                        encrypt_key, tmp_iv.data(), AES_ENCRYPT);

        delete encrypt_key;
        std::string output(reinterpret_cast<char*>(ciphertext),
                           reinterpret_cast<char*>(ciphertext+padded_text.size()));
        return output;
    }

    std::string AES::Decrypt(std::string input_text)
    {
        std::string ciphertext = input_text;
        AES_KEY* decrypt_key = new AES_KEY();
        AES_set_decrypt_key(key.data(), AES_KEYLENGTH, decrypt_key);

        unsigned char text[input_text.size()] = {0};
        std::array<uint8_t, AES_BLOCK_SIZE> tmp_iv = IV;
        AES_cbc_encrypt(reinterpret_cast<unsigned char*>(ciphertext.data()),
                        text, ciphertext.length(),
                        decrypt_key, tmp_iv.data(), AES_DECRYPT);
        delete decrypt_key;
        std::string output(reinterpret_cast<char*>(text));
        return output;
    }

};
