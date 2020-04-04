#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "search.hpp"
#include "match.hpp"

using namespace ruff::tester;
using namespace ruff::match;

void doStuff();

TEST_CASE("Testing AStar Search")
{
	CHECK(findEasyPath());
}
TEST_CASE("Matching")
{
	doStuff();
}


void doStuff()
{
	Matcher matcher(DetectorType::BRISK, MatcherType::BF, true);

	auto im1 = cv::imread("/home/chris/Downloads/1.png");
	auto im2 = cv::imread("/home/chris/Downloads/2.png");
	cv::Mat outImg;
	auto t = matcher.match(im1,im2);

	cv::Mat out;
	cv::drawMatches(im1, std::get<1>(t), im2, std::get<2>(t), std::get<0>(t), out);
	cv::imshow("TEST", out);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
