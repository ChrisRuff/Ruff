#pragma once
#include <string>

#include <openssl/sha.h>

std::string SHA256(const std::string& message);