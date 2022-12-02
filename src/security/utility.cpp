#include <ruff/security/utility.hpp>

namespace ruff::security
{
    mpz_class Euclid(mpz_class a, mpz_class b, mpz_class& x, mpz_class& y)
    {
        if(b == 0) { x=1; y=0; return a; }
        mpz_class x_, y_;
        auto gcd = Euclid(b, a%b, x_, y_);
        mpz_class divided = a/b;
        x = y_;
        y = x_ - divided.get_si() * y_;
        return gcd;
    }
    mpz_class Euclid(mpz_class a, mpz_class b)
    {
        mpz_class x,y;
        Euclid(a,b,x,y);

        return (x + (a*b)) % b;
    }
}
