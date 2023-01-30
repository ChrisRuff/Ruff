#pragma once
#include <math.h>

#include <ruff/core/models/tensor.hpp>

#define FLT_TYPE double

template<typename T = FLT_TYPE>
using Tensor = ruff::core::Tensor<T>;

namespace ruff::ai
{
    struct ActivationFunction
    {
        virtual FLT_TYPE Compute(FLT_TYPE value) = 0;
				virtual FLT_TYPE ComputeDerivative(FLT_TYPE value) = 0;
        virtual Tensor<FLT_TYPE> ComputeTensor(Tensor<FLT_TYPE> tensor)
        {
            for(size_t i = 0; i < tensor.rows(); ++i)
            {
                for(size_t j = 0; j < tensor.cols(); ++j)
                {
                    auto& val = tensor(i, j);
                    val = Compute(val);
                }
            }
            return tensor;
        };
				virtual Tensor<FLT_TYPE> ComputeDerivative(Tensor<FLT_TYPE> tensor)
				{
						for(size_t i = 0; i < tensor.rows(); ++i)
						{
								for(size_t j = 0; j < tensor.cols(); ++j)
								{
										auto& val = tensor(i, j);
										val = ComputeDerivative(val);
								}
						}
						return tensor;
				};
		    virtual ~ActivationFunction() {};
    };


    struct SIGMOID : public ActivationFunction
    {
        FLT_TYPE Compute(FLT_TYPE value) override { return 1.0 / (1+std::exp(-value)); }
        FLT_TYPE ComputeDerivative(FLT_TYPE value) override { return value * (1-value); };
    };

    struct RELU : public ActivationFunction
    {
        FLT_TYPE Compute(FLT_TYPE value) override { return std::max(static_cast<FLT_TYPE>(0.0), value); }
        FLT_TYPE ComputeDerivative(FLT_TYPE value) override { return value >= 0 ? 1 : 0; };
    };

    struct SOFTMAX : public ActivationFunction
    {
        FLT_TYPE Compute(FLT_TYPE value) override { return value; };
        FLT_TYPE ComputeDerivative(FLT_TYPE value) override { return value; };
        Tensor<FLT_TYPE> ComputeTensor(Tensor<FLT_TYPE> tensor) override
        {
            double denominator = 0;
            for(auto it = tensor.begin(); it != tensor.end(); ++it)
            {
                denominator += std::exp(*it);
            }

            for(auto it = tensor.begin(); it != tensor.end(); ++it)
            {
                *it = std::exp(*it) / denominator;
            }
            return tensor;
        }
    };
};