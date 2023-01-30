#include <ruff/photogrammetry/matching/feature.hpp>

#include <ruff/photogrammetry/utility/conversions.hpp>

namespace ruff::photogrammetry::matching
{
std::pair<std::vector<cv::KeyPoint>, cv::Mat> FeatureExtractor::Compute(const ruff::imgproc::Image& img)
{
		cv::Mat cv_img = convert(img);

		std::vector<cv::KeyPoint> kpts;
		cv::Mat desc;
		detector->detectAndCompute(cv_img, cv::noArray(), kpts, desc, false);

		return std::make_pair(kpts, desc);
}
ORBExtractor::ORBExtractor(size_t n_features, float scale, size_t levels, int edge_threshold, int first_level)
{
		detector = cv::ORB::create(n_features, scale, levels, edge_threshold, first_level);
}
}