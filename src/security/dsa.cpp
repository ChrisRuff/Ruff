#include <ruff/security/dsa.hpp>

#include <ruff/core/random.hpp>
#include <ruff/security/utility.hpp>
#include <ruff/security/sha256.hpp>

namespace ruff::security
{
    // Intermediate values
    mpz_class w, u1, u2, v;

    DSA::DSA(std::string p, std::string q) : p(p), q(q)
    {
        mpz_class z_size = (this->p-1);
        mpz_class exponent = (z_size/this->q);
        mpz_class max = (this->q-1);

        do {
            this->g = core::Rand(2UL, z_size.get_ui());
            mpz_powm(this->h.get_mpz_t(), this->g.get_mpz_t(), exponent.get_mpz_t(), this->p.get_mpz_t()); // h = g^((p-1)/q)
        }while(this->h == 1);

        this->x = core::Rand(2UL, max.get_ui());                                                          // x = rand[2, q-1]
        mpz_powm(this->y.get_mpz_t(), this->h.get_mpz_t(), this->x.get_mpz_t(), this->p.get_mpz_t());     // y = h^x mod p
    }

    DSASignature DSA::Sign(const std::string& message) const
    {
        mpz_class max = (this->q-1);
        mpz_class k = core::Rand(1UL, max.get_ui());
        mpz_class kp = Euclid(k, this->q);

        DSASignature sig;
        mpz_powm(sig.r.get_mpz_t(), h.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
        mpz_mod(sig.r.get_mpz_t(), sig.r.get_mpz_t(), q.get_mpz_t());
        if(sig.r == 0) return Sign(message);

        std::string digest_string = SHA256(message);
        mpz_class digest;
        mpz_import(digest.get_mpz_t(), digest_string.size(), 1, 1, 0, 0, digest_string.c_str());

        sig.s = kp * (digest + (this->x * sig.r));
        mpz_mod(sig.s.get_mpz_t(), sig.s.get_mpz_t(), q.get_mpz_t());

        if(sig.s == 0) return Sign(message);
        return sig;

    }
    bool DSA::Verify(const std::string& message, const DSASignature& sig) const
    {
        return Verify(message, sig, GetVerificationKey());
    }
    bool DSA::Verify(const std::string& message, const DSASignature& sig, const DSAVerificationKey& key)
    {
        w = Euclid(sig.s, key.q);

        std::string digest_string = SHA256(message);
        mpz_class digest;
        mpz_import(digest.get_mpz_t(), digest_string.size(), 1, 1, 0, 0, digest_string.c_str());

        u1 = w * digest;
        mpz_mod(u1.get_mpz_t(), u1.get_mpz_t(), key.q.get_mpz_t());

        u2 = sig.r * w;
        mpz_mod(u2.get_mpz_t(), u2.get_mpz_t(), key.q.get_mpz_t());

        mpz_class v1, v2;
        mpz_powm(v1.get_mpz_t(), key.h.get_mpz_t(), u1.get_mpz_t(), key.p.get_mpz_t());
        mpz_powm(v2.get_mpz_t(), key.y.get_mpz_t(), u2.get_mpz_t(), key.p.get_mpz_t());

        v = v1 * v2;
        mpz_mod(v.get_mpz_t(), v.get_mpz_t(), key.p.get_mpz_t());
        mpz_mod(v.get_mpz_t(), v.get_mpz_t(), key.q.get_mpz_t());

        if(v == sig.r) return true;
        return false;

    }

    mpz_class DSA::U1() const {
        return u1;
    }

    mpz_class DSA::U2() const {
        return u2;
    }

    mpz_class DSA::W() const {
        return w;
    }

    mpz_class DSA::V() const {
        return v;
    }
}