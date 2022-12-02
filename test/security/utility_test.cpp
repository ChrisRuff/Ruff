#include "doctest/doctest.h"

#include <ruff/security/utility.hpp>

TEST_SUITE("Utility")
{
    TEST_CASE("Euclidean Algorithm")
    {
        using namespace ruff::security;
        mpz_class a, b;
        a = "27";
        b = "392";
        mpz_class c = Euclid(a, b);
        CHECK(c == 363);
    }
}