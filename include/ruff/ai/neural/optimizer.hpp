#pragma once

#include <ruff/ai/loss_function.hpp>
#include <ruff/ai/activation_function.hpp>

namespace ruff::ai
{
    struct Weights
    {
        std::vector<double> w;
        double b;
    };
    struct TrainingData
    {
        std::vector<std::vector<double>> input_features;
        std::vector<double> labels;
    };

    template<class ActivationFunc>
    class GradientDescent
    {
    private:
        Weights& weights;
        double lr;

    public:
        GradientDescent(Weights& weights, double lr=0.05) : weights(weights), lr(lr) {}
        void Step(const TrainingData& data)
        {
            for(size_t _iter = 0; _iter < data.labels.size(); ++_iter)
            {
                std::vector<double> gradient;
						    gradient.Resize(weights.w.size() + 1);

                int sum = 0;
                for(size_t i = 0; i < weights.w.size(); ++i)
                {
                    sum += weights.w[i] * data.input_features[_iter][i];
                }
                sum += weights.b;
                double loss = ActivationFunc::Compute(sum) - data.labels[_iter];
                std::cout << std::to_string(loss) << std::endl;
                for(size_t i = 0; i < weights.w.size(); ++i)
                {
                    gradient[i] = loss * data.input_features[_iter][i];
                    weights.w[i] -= lr * gradient[i];
                }
                gradient.back() = ActivationFunc::Compute(sum) - data.labels.back();
                weights.b -= lr * gradient.back();
            }
        }
    };
};