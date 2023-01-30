#pragma once
#include <vector>
#include <utility>

#include <opencv2/features2d.hpp>

#include <ruff/imgproc/image.hpp>
namespace ruff::photogrammetry::matching
{
class FeatureExtractor
{
	protected:
		cv::Ptr<cv::Feature2D> detector;

	public:
		virtual std::pair<std::vector<cv::KeyPoint>, cv::Mat> Compute(const ruff::imgproc::Image&);
};

class ORBExtractor : public FeatureExtractor
{
	public:
		ORBExtractor(size_t n_features=500, float scale=1.2, size_t levels=8,
								 int edge_threshold=31, int first_level=0);
};
};
