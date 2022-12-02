#pragma once
#include <string>
#include <string_view>

#include <gmpxx.h>

namespace ruff::security
{
    struct DSAVerificationKey
    {
        const mpz_class& p{};
        const mpz_class& q{};
        const mpz_class& y{};
        const mpz_class& h{};
    };
    struct DSASigningKey
    {
        const mpz_class& x{};
    };
    struct DSASignature
    {
        mpz_class r{};
        mpz_class s{};
    };

    class DSA
    {
    private:
        mpz_class p, q;
        mpz_class g{};

        mpz_class h{}, y{};
        mpz_class x{};

    public:
        DSA(std::string p, std::string q);

        [[nodiscard]] DSASignature Sign(const std::string& message) const;
        [[nodiscard]] bool Verify(const std::string& message, const DSASignature& sig) const;
        static bool Verify(const std::string& message, const DSASignature& sig, const DSAVerificationKey& key);

        DSASigningKey GetSigningKey() const { return {x}; };
        DSAVerificationKey GetVerificationKey() const { return {p, q, y, h}; };

        mpz_class U1() const;
        mpz_class U2() const;
        mpz_class W() const;
        mpz_class V() const;

    };

};