#pragma once

#include <vector>
#include <memory>

#include <ruff/core/structs/tensor.hpp>
#include <ruff/ai/neural/activation_function.hpp>

namespace ruff::ai
{
    class Layer
    {
    protected:
        Tensor<FLT_TYPE> m_weights;
        Tensor<FLT_TYPE> m_bias;
        Tensor<FLT_TYPE> m_last_output;
    public:
        std::unique_ptr<ActivationFunction> m_func;
    public:
        Layer(int neurons_count, std::unique_ptr<ActivationFunction> activation_function) :
            m_weights(neurons_count, 1),
            m_bias(neurons_count, 1),
            m_last_output(0,0),
            m_func(std::move(activation_function))
        {
            m_weights.Randomize(-1.0f, 1.0f);
            m_bias.Randomize(-1.0f, 1.0f);
        };
        Tensor<FLT_TYPE> Compute(const Tensor<FLT_TYPE>& input)
        {
            m_last_output = (m_weights * input) + m_bias;
            m_last_output = m_func->ComputeTensor(m_last_output);
			return m_last_output;
        };
        void UpdateWeights(const Tensor<FLT_TYPE>& gradient)
        {
            m_weights -= gradient;
        }
        void UpdateBias(const Tensor<FLT_TYPE>& gradient)
        {
            m_bias -= gradient;
        }

        Tensor<FLT_TYPE> LastOutput() const { return m_last_output; };
				const Tensor<FLT_TYPE>& Weights() const { return m_weights; };
				const Tensor<FLT_TYPE>& Bias() const { return m_bias; };

        size_t InputSize() const { return m_weights.cols(); };
        size_t OutputSize() const { return m_weights.rows(); };

        void SetOutputSize(size_t size)
		{
			m_weights.Resize(size, m_weights.cols());
			m_bias.Resize(size, m_bias.cols());
		};
        void SetInputSize(size_t size)
		{
			m_weights.Resize(m_weights.rows(), size);
		};
    };
};