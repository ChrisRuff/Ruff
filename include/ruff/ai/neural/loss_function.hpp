#pragma once

namespace ruff::ai
{
    template<typename T>
    struct LossFunction
    {
        virtual T Loss(T ground_truth, T predicted) = 0;
        virtual T LossDerivative(T ground_truth, T predicted) = 0;
		    virtual Tensor<T> Loss(const Tensor<T>& ground_truth, const Tensor<T>& predicted)
		    {
				    Tensor<T> out(ground_truth.rows(), ground_truth.cols());
				    for(size_t i = 0; i < out.Size(); ++i)
				    {
						    out[i] = Loss(ground_truth[i], predicted[i]);
				    }
				    return out;
		    };
				virtual Tensor<T> LossDerivative(const Tensor<T>& ground_truth, const Tensor<T>& predicted)
		    {
						Tensor<T> out(ground_truth.rows(), ground_truth.cols());
						for(size_t i = 0; i < out.Size(); ++i)
						{
								out[i] = LossDerivative(ground_truth[i], predicted[i]);
						}
						return out;
		    }

		    virtual ~LossFunction() = default;
    };

    template<typename T>
    struct MeanSquared : public LossFunction<T>
    {
				using LossFunction<T>::Loss;
				using LossFunction<T>::LossDerivative;
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
				using LossFunction<T>::Loss;
				using LossFunction<T>::LossDerivative;
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
