#pragma once
#include <gmpxx.h>

#include <ruff/core/random.hpp>
namespace ruff::security
{
    class DiffieHellman
    {
    private:
        mpz_class m_mod;
        mpz_class m_g;

        mpz_class m_exponent;
        mpz_class m_key;
    public:
        DiffieHellman(std::string mod, std::string g) :
            m_mod(mod), m_g(g),
            m_exponent(core::Rand(1ul, m_mod.get_ui())),
            m_key(0)
        {
        }
        mpz_class Send()
        {
            mpz_class out;
            mpz_powm(out.get_mpz_t(), m_g.get_mpz_t(), m_exponent.get_mpz_t(), m_mod.get_mpz_t());
            return out;
        }
        void Recieve(mpz_class input)
        {
            mpz_powm(m_key.get_mpz_t(), input.get_mpz_t(), m_exponent.get_mpz_t(), m_mod.get_mpz_t());
        }
        mpz_class Key() const { return m_key; }
    };
}