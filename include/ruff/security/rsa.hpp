#pragma once

#include <string>
#include <tuple>
#include <gmp.h>
#include <gmpxx.h>

namespace ruff::security
{
class RSA
{
public:
    enum class DECRYPT_MODE
    {
        REGULAR,
        CRT
    };
private:
	mpz_class p, q;
	mpz_class phi{}, n{};
	mpz_class d{}, e{};

  // CRT
  mpz_class qp{}, pp{}, dp{}, dq{};

public:
	RSA(std::string p, std::string q);

	std::string Encrypt(std::string text);
    static std::string Encrypt(std::string text, mpz_class e, mpz_class n);

	std::string Decrypt(std::string ciphertext, DECRYPT_MODE decrypt_mode=DECRYPT_MODE::REGULAR);

	const std::tuple<mpz_class&, mpz_class&> GetPublicKey();
	const std::tuple<mpz_class&, mpz_class&, mpz_class&> GetPrivateKey();

    friend long long GCD(long long a, long long b);
    friend mpz_class Euclid(mpz_class a, mpz_class b);
};
}// namespace ruff::security