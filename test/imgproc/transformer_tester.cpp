#include "doctest/doctest.h"

#include <string>
#include <iostream>

#include <ruff/imgproc/image_io.hpp>
#include <ruff/imgproc/ascii.hpp>

TEST_SUITE("Transformers")
{
    using namespace ruff::imgproc;
    TEST_CASE("ASCII transform")
    {
        Image mario_image = ImageIO::read(std::filesystem::path(DATA_DIR) / "mario.png");
        ASCII ascii(mario_image, true);

        ascii.Compute();
				ascii.GetData();
    }
}
