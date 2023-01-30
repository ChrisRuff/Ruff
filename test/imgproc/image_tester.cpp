#include "doctest/doctest.h"

#include <filesystem>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <ruff/imgproc/image_io.hpp>
#include <ruff/imgproc/image_manipulator.hpp>
#include <ruff/imgproc/image.hpp>

using namespace ruff::imgproc;
bool Compare(const Image& img1, const cv::Mat& img2)
{
		if(img1.Width() != img2.cols)
				return false;
		if(img1.Height() != img2.rows)
				return false;

		for(size_t i = 0; i < img1.Height(); ++i)
		{
				for(size_t j = 0; j < img1.Width(); ++j)
				{
						Pixel p = img1.Get(j, i);
						Pixel o;
						switch(img2.channels())
						{
								case 1: {
								    o = Pixel(img2.at<uchar>(i, j));
								    o.a = p.a;
								    break;
						    }
								case 3: {
										auto vec = img2.at<cv::Vec3b>(i, j);
								    o = Pixel(vec[2], vec[1], vec[0], p.a);
								    break;
								}
								case 4: {
										auto vec = img2.at<cv::Vec4b>(i, j);
										o = Pixel(vec[2], vec[1], vec[0], vec[3]);
										break;
								}
						}
						if(p != o)
						{
								return false;
						}
				}
		}
		return true;
}
TEST_SUITE("Image Tests")
{
	TEST_CASE("Read/Write Image")
	{
		auto data_dir = std::filesystem::path(DATA_DIR);
		SUBCASE("PNG")
		{
				auto image = ImageIO::read(data_dir / "1.png");
				cv::Mat cv_img = cv::imread(data_dir / "1.png");
				CHECK(Compare(image, cv_img));

				ImageIO(image).write(data_dir / "1_.png");
		}
		SUBCASE("PPM")
		{
				auto image = ImageIO::read(data_dir / "1.ppm");
				cv::Mat cv_img = cv::imread(data_dir / "1.png"); // opencv cant open the ppm
				CHECK(Compare(image, cv_img));

				ImageIO(image).write(std::filesystem::path(DATA_DIR) / "1_.ppm");
		}
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
