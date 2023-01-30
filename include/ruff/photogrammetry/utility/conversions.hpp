#pragma once
#include <opencv2/opencv.hpp>

#include <ruff/core/models/tensor.hpp>
#include <ruff/imgproc/image.hpp>

namespace ruff
{
		template<typename T>
		cv::Mat convert(core::Tensor<T>&);

		template<typename T>
		cv::Mat convert(const core::Tensor<T>&);

		cv::Mat convert(const imgproc::Image&);
    imgproc::Image convert(cv::Mat);
};