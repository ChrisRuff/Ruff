#include "doctest/doctest.h"

#include <vector>

#include <ruff/core/models/tensor.hpp>
TEST_SUITE("Tensor Tests")
{
    using namespace ruff::core;
    TEST_CASE("Iterator")
    {

        {
            Tensor<double> m1(4, 1);
            m1(0,0) = 1;
            m1(1,0) = 2;
            m1(2,0) = 3;
            m1(3,0) = 4;
            auto it = m1.begin();
            for(size_t i = 0; i < m1.rows(); ++i)
            {
                for(size_t j = 0; j < m1.cols(); ++j)
                {
                    CHECK(*it == m1(i, j));
                    CHECK(it != m1.end());
                    ++it;
                }
            }
            CHECK(it == m1.end());
        }

        {
            Tensor<double> m2(1, 4);
            m2(0,0) = 5;
            m2(0,1) = 5;
            m2(0,2) = 5;
            m2(0,3) = 5;
            auto it = m2.begin();
            for(size_t i = 0; i < m2.rows(); ++i)
            {
                for(size_t j = 0; j < m2.cols(); ++j)
                {
                    CHECK(*it == m2(i, j));
                    CHECK(it != m2.end());
                    it++;
                }
            }
            CHECK(it == m2.end());
        }
    }
    TEST_CASE("Correct row/col after Multiplcation")
    {
        Tensor<double> m1(4, 1);
        m1(0,0) = 1;
        m1(1,0) = 2;
        m1(2,0) = 3;
        m1(3,0) = 4;

        Tensor<double> m2(1, 4);
        m2(0,0) = 5;
        m2(0,1) = 5;
        m2(0,2) = 5;
        m2(0,3) = 5;

        auto m3 = m1 * m2;
        auto m4 = m2 * m1;
        CHECK(m3.rows() == 4);
        CHECK(m3.cols() == 4);

        CHECK(m4.rows() == 1);
        CHECK(m4.cols() == 1);
    }
    TEST_CASE("Correct data")
    {
        Tensor<int> m1(2, 3);
        m1(0,0) = 1;
        m1(0,1) = 2;
        m1(0,2) = 3;
        m1(1,0) = 4;
        m1(1,1) = 5;
        m1(1,2) = 6;

        Tensor<int> m2(3, 2);
        m2(0,0) = 7;
        m2(0,1) = 8;
        m2(1,0) = 9;
        m2(1,1) = 10;
        m2(2,0) = 11;
        m2(2,1) = 12;

        auto m3 = m1 * m2;
        CHECK(m3(0,0) == 58);
        CHECK(m3(0,1) == 64);
        CHECK(m3(1,0) == 139);
        CHECK(m3(1,1) == 154);
    }
    TEST_CASE("Invalid Matrix sizes")
    {
        Tensor<int> m1(2, 2);
        m1(0,0) = 1;
        m1(0,1) = 2;
        m1(1,0) = 4;
        m1(1,1) = 5;

        Tensor<int> m2(3, 2);
        m2(0,0) = 7;
        m2(0,1) = 8;
        m2(1,0) = 9;
        m2(1,1) = 10;
        m2(2,0) = 11;
        m2(2,1) = 12;

        try
        {
            m1 * m2;
            CHECK(false);
        }
        catch(...) {}
    }
}
