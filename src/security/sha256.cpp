#include <ruff/security/sha256.hpp>

std::string SHA256(const std::string& message)
{
    unsigned char buffer[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, message.c_str(), message.size());
    SHA256_Final(buffer, &sha256);

    char digest[256];
    for(int i = 0; i < 256; ++i)
    {
        sprintf(digest + i, "%c", buffer[i]);
    }
    digest[256] = 0;

    return std::string(digest, 256);
}

