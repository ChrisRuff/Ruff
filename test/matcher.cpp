#include "doctest/doctest.h"

#include "match.hpp"
#include <iostream>

using namespace ruff::match;

void Match();

TEST_CASE("Matching")
{

	// Don't show the matches
	bool display = false;

	// Enforces that all detector types can at least be ran
	auto im1 = cv::imread("../test/1.png");
	auto im2 = cv::imread("../test/1.png");
	for(DetectorType dType : DetectorTypes)
	{
		for(MatcherType mType : MatcherTypes)
		{
			for(const bool knn : { false, true })
			{
				if(dType == DetectorType::DAISY && mType == MatcherType::BF)
					continue;
				Matcher matcher(dType, mType, knn);
				if(display)
				{
					cv::Mat  outImg;
					kptMatch matchResult;
					matchResult = matcher.match(im1, im2);
					cv::Mat out;
					cv::drawMatches(im1, std::get<1>(matchResult), im2, std::get<2>(matchResult), std::get<0>(matchResult), out);
					cv::imshow("TEST", out);
					cv::waitKey(0);
					cv::destroyAllWindows();
				}
			}
		}
	}
};
