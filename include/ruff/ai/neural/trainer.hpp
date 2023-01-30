#pragma once
#include <ruff/ai/neural/neural_network.hpp>
#include <ruff/ai/neural/predictor.hpp>
namespace ruff::ai
{
    using TrainingData = std::vector<std::pair<core::Tensor<FLT_TYPE>, core::Tensor<FLT_TYPE>>>;

    class Trainer : public Predictor
    {
    public:
        Trainer(NeuralNetwork& network) : Predictor(network)
        {

        };
        void Train(TrainingData input_data)
        {
            auto& settings = m_network.settings;
            auto& layers = m_network.GetLayers();
				    MeanSquared<FLT_TYPE> cost_func;

            for(size_t _iter = 0; _iter < m_network.settings.max_iter; ++_iter)
            {
                for(auto& [training_input, training_label] : input_data)
                {
                    Tensor<> output = Predict(training_input);
                    Tensor<> expected_output = training_label;
                    Tensor<> delta = output - expected_output;

								    auto next_layer = layers.rbegin();
										auto prev_layer = layers.rbegin();

										Tensor<> w_gradient;
										Tensor<> b_gradient;
                    for(auto layer_it = layers.rbegin(); layer_it != layers.rend(); ++layer_it)
                    {
												prev_layer++;

										    if(layer_it == layers.rend()-1)
										    {
														w_gradient = (training_input *
																					delta *
																					layer_it->m_func->ComputeDerivative(output) *
																					settings.learning_rate).Transpose();
										    }
										    else
										    {
														w_gradient = (layer_it->LastOutput() *
																					delta *
																					layer_it->m_func->ComputeDerivative(output) *
																					settings.learning_rate).Transpose();
										    }
												b_gradient = delta *
										                 layer_it->m_func->ComputeDerivative(output) *
										                 settings.learning_rate;

												layer_it->UpdateWeights(w_gradient);
												layer_it->UpdateBias(b_gradient);
										    next_layer = layer_it;
                    }

                }
            }
        }
    };
};