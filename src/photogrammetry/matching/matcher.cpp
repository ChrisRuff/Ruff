#include <ruff/photogrammetry/matching/matcher.hpp>

#include <ruff/photogrammetry/utility/conversions.hpp>

namespace ruff::photogrammetry::matching
{
std::vector<cv::DMatch> Matcher::Compute(const cv::Mat desc1, const cv::Mat desc2)
{
		std::vector<cv::DMatch> matches;
		matcher->match(desc1, desc2, matches);

		return matches;
}
BFMatcher::BFMatcher()
{
		matcher = cv::BFMatcher::create();
}
};
