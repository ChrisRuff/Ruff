#include "doctest/doctest.h"

#include <filesystem>
#include <ruff/imgproc/image_io.hpp>
#include <ruff/imgproc/image_manipulator.hpp>
#include <ruff/imgproc/image.hpp>

TEST_SUITE("Image Tests")
{
    using namespace ruff::imgproc;
	TEST_CASE("Read Image")
	{
		auto image = ImageIO::read(std::filesystem::path(DATA_DIR) / "1.png");

		ImageIO(image).write(std::filesystem::path(DATA_DIR) / "1.ppm");
	}
	TEST_CASE("Rotate Image")
	{
		auto image = ImageIO::read(std::filesystem::path(DATA_DIR) / "1.png");
        double theta = M_PI*2;

		//image = ImageManipulator(image).Rotate(M_PI / 3);
        image = ImageManipulator(image).Rotate(theta);
        //image = ImageManipulator(image).Rotate(M_PI /2);
		ImageIO(image).write(std::filesystem::path(DATA_DIR) / "rotated.ppm");

        image = ImageIO::read(std::filesystem::path(DATA_DIR) / "2.png");

        //image = ImageManipulator(image).Rotate(M_PI / 3);
        image = ImageManipulator(image).Rotate(theta);
        //image = ImageManipulator(image).Rotate(M_PI /2);
        ImageIO(image).write(std::filesystem::path(DATA_DIR) / "rotated2.ppm");
	}
}
