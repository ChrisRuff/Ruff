#pragma once
#include <vector>
#include <memory>

#include <ruff/ai/neural/layer.hpp>
#include <ruff/ai/neural/neural_network.hpp>
namespace ruff::ai
{
    class Predictor
    {
    protected:
        NeuralNetwork& m_network;
    public:
        Predictor(NeuralNetwork& network) : m_network(network)
        {
        };
        Tensor<FLT_TYPE> Predict(Tensor<FLT_TYPE> input)
        {
            auto& layers = m_network.GetLayers();
            Tensor<FLT_TYPE> value_tensor = input;
            for(size_t i = 0; i < layers.size(); ++i)
            {
                value_tensor = layers[i].Compute(value_tensor);
            }
            return value_tensor;
        }
    };
};