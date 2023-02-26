#pragma once
#include <concepts>
#include <any>
#include <vector>
#include <memory>

#include <ruff/ai/neural/activation_function.hpp>
#include <ruff/ai/neural/layer.hpp>
#include <ruff/ai/neural/loss_function.hpp>

namespace ruff::ai
{
    struct NeuralNetworkSettings
    {
        FLT_TYPE learning_rate = 0.01;
        size_t max_iter = 10000;
		    size_t batch_size = 1;
		    size_t log_interval = 1000;

        std::unique_ptr<ActivationFunction> activation_func = std::make_unique<SIGMOID>();
        std::unique_ptr<LossFunction<FLT_TYPE>> loss_func = std::make_unique<MeanSquared<FLT_TYPE>>();
    };
    class NeuralNetwork
    {
    public:
        NeuralNetworkSettings settings;
    private:
				size_t m_input_size;
        std::vector<Layer> layers{};

    public:
        NeuralNetwork(size_t input_size, NeuralNetworkSettings& settings)
            : settings(std::move(settings)), m_input_size(input_size)
        {
        }
        std::vector<Layer>& GetLayers() { return layers; };
				Layer& GetInputLayer() { return *layers.begin(); };
        Layer& GetOutputLayer() { return layers.back(); };

    public:
        template<typename T> requires std::is_base_of_v<ActivationFunction, T>
        Layer& AddLayer(size_t neuron_size)
        {
            layers.emplace_back(neuron_size, std::make_unique<T>());
            if(layers.size() > 1)
            {
								size_t previous = layers.size()-2;
                layers.back().SetInputSize(layers[previous].OutputSize());
            }
            else
            {
                layers.back().SetInputSize(m_input_size);
            }
            layers.back().SetOutputSize(neuron_size);
				    return layers.back();
        }
    };
};