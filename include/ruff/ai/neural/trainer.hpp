#pragma once
#include <ruff/core/logger.hpp>
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
            for(size_t _iter = 0; _iter <= m_network.settings.max_iter; ++_iter)
            {
								Tensor<> error(input_data[0].second.Size(), 1);
                for(auto& [training_input, training_label] : input_data)
                {
                    Tensor<> output = Predict(training_input);
										error += cost_func.Loss(output, training_label);

								    auto next_layer = layers.rbegin();
										auto prev_layer = layers.rbegin();

										Tensor<> w_gradient;
										Tensor<> b_gradient;
										Tensor<> delta;
								    // http://neuralnetworksanddeeplearning.com/chap2.html
                    for(auto layer_it = layers.rbegin(); layer_it != layers.rend(); ++layer_it)
                    {
												prev_layer++;

										    if(layer_it == layers.rbegin())
										    {
												    delta = (layer_it->LastOutput() - training_label);
										    }
										    else
										    {
												    delta = (next_layer->Weights().Transpose() * delta);
										    }
										    delta = delta.Multiply(layer_it->m_func->ComputeDerivative(layer_it->LastOutput()));

												w_gradient = delta *
																		 (layer_it == layers.rend()-1 ? training_input : prev_layer->LastOutput()).Transpose() *
																		 settings.learning_rate;

										    b_gradient = delta *
										                 settings.learning_rate;


										    layer_it->UpdateWeights(w_gradient);
												layer_it->UpdateBias(b_gradient);

										    next_layer = layer_it;
                    }
                }
								if(_iter % settings.log_interval == 0)
						    {
										std::string msg = "##################################\n";
								    msg += ("Iteration: " + std::to_string(_iter));

								    double avg_error = 0;
								    for(size_t i = 0; i < error.Size(); ++i)
								    {
										    avg_error += error[i];
								    }
								    avg_error /= error.Size();
								    msg += "\nCost: " + std::to_string(avg_error);
										msg += "\n##################################";
								    ruff::logInfo(msg);
						    }
            }
        }
    };
};