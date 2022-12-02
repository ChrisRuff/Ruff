#include "doctest/doctest.h"

#include <vector>

#include <ruff/ai/neural/activation_function.hpp>
#include <ruff/ai/neural/loss_function.hpp>
TEST_SUITE("Activation Functions")
{
    using namespace ruff::ai;
    TEST_CASE("Sigmoid")
    {
        SIGMOID sigmoid;
        CHECK_EQ(sigmoid.Compute(2), doctest::Approx(0.880797));
        CHECK_EQ(sigmoid.Compute(1), doctest::Approx(0.731058));
        CHECK_EQ(sigmoid.Compute(0), doctest::Approx(0.5));
    }
    TEST_CASE("Softmax")
    {
        Tensor<FLT_TYPE> values(1, 3);
        values(0, 0) = 3;
        values(0, 1) = 4;
        values(0, 2) = 1;

        SOFTMAX softmax;
        Tensor<FLT_TYPE> output = softmax.ComputeTensor(values);

        CHECK_EQ(output(0, 0), doctest::Approx(0.259496));
        CHECK_EQ(output(0, 1), doctest::Approx(0.705384));
        CHECK_EQ(output(0, 2), doctest::Approx(0.035119));
    }
    TEST_CASE("RELU")
    {
        RELU relu;
        CHECK_EQ(relu.Compute(-2), doctest::Approx(0));
        CHECK_EQ(relu.Compute(-1), doctest::Approx(0));
        CHECK_EQ(relu.Compute(0), doctest::Approx(0));
        CHECK_EQ(relu.Compute(1), doctest::Approx(1));
        CHECK_EQ(relu.Compute(2), doctest::Approx(2));
    }
}
TEST_SUITE("Loss Functions")
{
    using namespace ruff::ai;
    TEST_CASE("Cross Entropy Loss")
    {
        CrossEntropy<FLT_TYPE> loss;
        CHECK_EQ(loss.Loss(1, 0.7), doctest::Approx(0.35667));
        CHECK_EQ(loss.Loss(0, 0.7), doctest::Approx(1.20397));

				// Not implemented yet
				CHECK_THROWS(loss.LossDerivative(1, 0.7) == doctest::Approx(0));
				CHECK_THROWS(loss.LossDerivative(0, 0.7) == doctest::Approx(0));
    }
		TEST_CASE("Mean Squared")
		{
				MeanSquared<FLT_TYPE> loss;
				CHECK_EQ(loss.Loss(1, 0.7), doctest::Approx(0.09));
				CHECK_EQ(loss.Loss(0, 0.7), doctest::Approx(0.49));

				CHECK_EQ(loss.LossDerivative(1, 0.7), doctest::Approx(0.6));
				CHECK_EQ(loss.LossDerivative(0, 0.7), doctest::Approx(-1.4));
		}
}
