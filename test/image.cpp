#include "doctest/doctest.h"

#include <filesystem>
#include "ruff/ui/image.hpp"

TEST_SUITE("Image Tests")
{
	TEST_CASE("Read Image")
	{
		auto image = ruff::ui::Image::read(std::filesystem::path(DATA_DIR) / "1.png");
		image.write(std::filesystem::path(DATA_DIR) / "1.ppm");
	}
	TEST_CASE("Rotate Image")
	{
		auto image = ruff::ui::Image::read(std::filesystem::path(DATA_DIR) / "1.png");
		image = image.rotate(M_PI/3);
		image.write(std::filesystem::path(DATA_DIR) / "rotated.ppm");
	}
}
