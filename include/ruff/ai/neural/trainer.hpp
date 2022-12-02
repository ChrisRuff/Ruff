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

            for(size_t _iter = 0; _iter < m_network.settings.max_iter; ++_iter)
            {
                for(auto& [training_input, training_label] : input_data)
                {
                    Tensor<FLT_TYPE> output = Predict(training_input);
                    Tensor<FLT_TYPE> expected_output = training_label;
                    Tensor<FLT_TYPE> delta = output - expected_output;

                    for(auto layer_it = layers.rbegin(); layer_it != layers.rend(); ++layer_it)
                    {
												Tensor<FLT_TYPE> w_gradient(layer_it->OutputSize(), layer_it->InputSize());
												Tensor<FLT_TYPE> b_gradient(layer_it->OutputSize(), 1);
												if(layer_it == layers.rbegin())
					              {
														w_gradient += (training_input *
												                  delta *
																					layer_it->m_func->ComputeDerivative(output) *
																					settings.learning_rate).Transpose();
													  b_gradient += delta *
																				  layer_it->m_func->ComputeDerivative(output) *
																				  settings.learning_rate;
					              }
					              else
					              {
					              }
												layer_it->UpdateWeights(w_gradient);
												layer_it->UpdateBias(b_gradient);
                    }

                }
            }
        }
    };
};