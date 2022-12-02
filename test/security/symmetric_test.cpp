#include "doctest/doctest.h"

#include <iostream>

#include <ruff/security/symmetric.hpp>
TEST_SUITE("Symmetric")
{
    TEST_CASE("AES")
    {
        ruff::security::AES aes = ruff::security::AES();
        std::string msg = "Hello World";
        std::string ciphertext = aes.Encrypt(msg);
        std::string text = aes.Decrypt(ciphertext);
        std::cout << msg << "\t->\t" << ciphertext << "\t->\t" << text << std::endl;

        CHECK_EQ(msg, text);
    }
}

