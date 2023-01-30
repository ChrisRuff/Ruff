#pragma once
#include <vector>
#include <utility>

#include <opencv2/features2d.hpp>

#include <ruff/imgproc/image.hpp>

namespace ruff::photogrammetry::matching
{
class Matcher
{
	protected:
		cv::Ptr<cv::DescriptorMatcher> matcher;

	public:
		virtual std::vector<cv::DMatch> Compute(const cv::Mat desc1, const cv::Mat desc2);
};
class BFMatcher : public Matcher
{
	public:
		BFMatcher();
};

};