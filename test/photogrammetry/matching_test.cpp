#include <doctest/doctest.h>

#include <filesystem>

#include <ruff/imgproc/image_io.hpp>

#include <ruff/photogrammetry/matching/feature.hpp>
#include <ruff/photogrammetry/matching/matcher.hpp>
#include <ruff/photogrammetry/utility/conversions.hpp>

TEST_SUITE("Feature Matching")
{
		using namespace ruff::photogrammetry::matching;
		using namespace ruff::imgproc;
		TEST_CASE("ORB Extraction - BF Matching")
		{
				std::filesystem::path data_dir = std::filesystem::path(DATA_DIR);
				Image img1 = ImageIO::read(data_dir / "1.png");
				Image img2 = ImageIO::read(data_dir / "2.png");
				auto orb = ORBExtractor();

				auto [kpts1, desc1] = orb.Compute(img1);
				auto [kpts2, desc2] = orb.Compute(img2);

				auto bf = BFMatcher();
				auto matches = bf.Compute(desc1, desc2);

				cv::Mat out;
				cv::drawMatches(ruff::convert(img1), kpts1, ruff::convert(img2), kpts2, matches, out);
				cv::imwrite("/tmp/test.png", out);
		}
}
