#pragma once

namespace ruff::ai
{
    template<typename T>
    struct LossFunction
    {
        virtual T Loss(T ground_truth, T predicted) = 0;
        virtual T LossDerivative(T ground_truth, T predicted) = 0;

		    virtual ~LossFunction() = default;
    };

    template<typename T>
    struct MeanSquared : public LossFunction<T>
    {
        T Loss(T ground_truth, T predicted) override
        {
            return std::pow(ground_truth - predicted, 2);
        }
        T LossDerivative(T ground_truth, T predicted) override
        {
            return (ground_truth - predicted) * 2;
        }
    };

    template<typename T>
    struct CrossEntropy : public LossFunction<T>
    {
        T Loss(T ground_truth, T predicted)
        {
            return -(ground_truth * std::log(predicted) + (1-ground_truth) * std::log(1-predicted));
        }
        T LossDerivative(T ground_truth, T predicted) override
        {
				    (void)ground_truth;
						(void)predicted;
            throw std::string("NOT IMPLEMENTED");
        }
    };
}
