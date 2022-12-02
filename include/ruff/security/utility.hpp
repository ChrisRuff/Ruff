#pragma once

#include <gmpxx.h>
namespace ruff::security
{
    mpz_class Euclid(mpz_class a, mpz_class b, mpz_class &x, mpz_class &y);
    mpz_class Euclid(mpz_class a, mpz_class b);

    template<typename T>
    T Euclid(T a, T b)
    {
        mpz_class ma = a;
        mpz_class mb = b;
        mpz_class out = Euclid(ma, mb);
        if(std::is_signed_v<T>)
        {
            return out.get_si();
        }
        else
        {
            return out.get_ui();
        }
    }
}
