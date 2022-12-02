#include <bitset>
#include <algorithm>

#include <ruff/security/rsa.hpp>
#include <ruff/security/utility.hpp>

namespace ruff::security
{
RSA::RSA(std::string p, std::string q) : p(p), q(q)
{
    this->n = this->p * this->q;
    this->phi = (this->p-1) * (this->q-1);

    this->e = 65537;
    this->d = 0;
    for(unsigned long i = 0; i <= this->n; ++i)
    {
        mpz_class x = 1 + (i*this->phi);
        if(x % this->e.get_ui() == 0)
        {
            this->d = x / this->e;
            break;
        }
    }
    pp = Euclid(this->p, this->q);
    qp = Euclid(this->q, this->p);
    dp = d % (this->p-1);
    dq = d % (this->q-1);
}

std::string RSA::Encrypt(std::string text)
{
	return Encrypt(text, e, n);
}
std::string RSA::Encrypt(std::string text, mpz_class e, mpz_class n)
{
    mpz_class bits;
    mpz_import(bits.get_mpz_t(), text.size(), 1, 1, 0, 0, text.c_str());
    mpz_powm(bits.get_mpz_t(), bits.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

    return bits.get_str(10);
}

std::string RSA::Decrypt(std::string ciphertext, DECRYPT_MODE decrypt_mode)
{
    mpz_class bits;
    bits.set_str(ciphertext.c_str(), 10);
    if(decrypt_mode == DECRYPT_MODE::REGULAR)
    {
        mpz_powm(bits.get_mpz_t(), bits.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
    }
    else if(decrypt_mode == DECRYPT_MODE::CRT)
    {
        mpz_class cq, cp, mq, mp;
        cp = bits % p;
        cq = bits % q;
        mpz_powm(mp.get_mpz_t(), cp.get_mpz_t(), dp.get_mpz_t(), p.get_mpz_t());
        mpz_powm(mq.get_mpz_t(), cq.get_mpz_t(), dq.get_mpz_t(), q.get_mpz_t());
        mpz_class p1 = (mp * q * qp);
        mpz_class p2 = (mq * p * pp);
        bits = (p1 + p2) % n;
    }

    char buffer[3072] = {'\0'};
    mpz_export(buffer, NULL, 1, 1, 0, 0, bits.get_mpz_t());
	return std::string(buffer);
}

const std::tuple<mpz_class&, mpz_class&> RSA::GetPublicKey()
{
	return std::tie(e, n);
}

const std::tuple<mpz_class&, mpz_class&, mpz_class&> RSA::GetPrivateKey()
{
	return std::tie(d, p, q);
}



}// namespace ruff::security
