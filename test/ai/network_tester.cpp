#include "doctest/doctest.h"

#include <vector>

#include <ruff/core/models/tensor.hpp>

#include <ruff/ai/neural/neural_network.hpp>
#include <ruff/ai/neural/trainer.hpp>
#include <ruff/ai/neural/predictor.hpp>

TEST_SUITE("Neural Networks")
{
    using namespace ruff::ai;
    TEST_CASE("And Gate")
    {
        std::vector<std::pair<Tensor<FLT_TYPE>, Tensor<FLT_TYPE>>> training_data{
                {{0.f, 0.f}, {0.f}},
                {{0.f, 1.f}, {0.f}},
                {{1.f, 0.f}, {0.f}},
                {{1.f, 1.f}, {1.f}}
        };

        NeuralNetworkSettings settings;
        NeuralNetwork network(2, settings);
        network.AddLayer<RELU>(1);

        Trainer trainer(network);
        trainer.Train(training_data);

        Predictor predictor(network);
        Tensor<FLT_TYPE> input(2,1);
        for(size_t i = 0; i < 2; ++i)
        {
            input(0, 0) = i;
            for(size_t j = 0; j < 2; ++j)
            {
                input(1, 0) = j;
                CHECK_EQ(std::round(predictor.Predict(input)(0,0)), i & j);
            }
        }
    }
    TEST_CASE("Or Gate")
    {
				std::vector<std::pair<Tensor<FLT_TYPE>, Tensor<FLT_TYPE>>> training_data{
                {{0.f, 0.f}, {0.f}},
                {{0.f, 1.f}, {1.f}},
                {{1.f, 0.f}, {1.f}},
                {{1.f, 1.f}, {1.f}}
        };

        NeuralNetworkSettings settings;
        NeuralNetwork network(2, settings);
        network.AddLayer<RELU>(1);

        Trainer trainer(network);
        trainer.Train(training_data);

        Predictor predictor(network);

        Tensor<FLT_TYPE> input(2,1);
        for(size_t i = 0; i < 2; ++i)
        {
            input(0, 0) = i;
            for(size_t j = 0; j < 2; ++j)
            {
                input(1, 0) = j;
                CHECK_EQ(std::round(predictor.Predict(input)(0,0)), i | j);
            }
        }
    }
	TEST_CASE("Xor Gate")
	{
	  std::vector<std::pair<Tensor<FLT_TYPE>, Tensor<FLT_TYPE>>> training_data{
			{{0.f, 0.f}, {0.f}},
			{{0.f, 1.f}, {1.f}},
			{{1.f, 0.f}, {1.f}},
			{{1.f, 1.f}, {0.f}}
		};

		NeuralNetworkSettings settings;
		NeuralNetwork network(2, settings);
		network.AddLayer<RELU>(2);
		network.AddLayer<RELU>(1);

		Trainer trainer(network);
		trainer.Train(training_data);

		Predictor predictor(network);
		Tensor<FLT_TYPE> input(2,1);
		for(size_t i = 0; i < 2; ++i)
		{
			input(0, 0) = i;
			for(size_t j = 0; j < 2; ++j)
			{
				input(1, 0) = j;
				CHECK_EQ(std::round(predictor.Predict(input)(0,0)), i ^ j);
			}
		}
	}
}
