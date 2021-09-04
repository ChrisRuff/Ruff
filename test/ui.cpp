#include "doctest/doctest.h"
#include <iostream>
#include <chrono>
#include <thread>

#include "ui/ui.hpp"
using namespace ruff::ui;

class TestEngine : public Engine
{
public:
	TestEngine(const uint16_t width, const uint16_t height, const std::string& title = "Ruff Pixel Engine")
	  : Engine(height, width, title) {}
	int count{};
	std::vector<ruff::Point2D<uint16_t>> newPixels{};
	ruff::ui::Image mario_image{};

	virtual void onCreate() override
	{
		count = 0;

		// Difference between ninja test(first) and running the binary(second)
		mario_image = ruff::ui::Image::read(std::filesystem::path(DATA_DIR) / "mario.png"); 
	}
	virtual void onUpdate(double deltaTime) override
	{
		clearScreen();
		drawLine(0, 0, count, count, Pixel(128, 128, 128, 1));
		drawCircle(count, count, 50, RED, true);
		drawCircle(mouse.mouse_x, mouse.mouse_y, 10, Pixel(255, 255, 255, 1), true);
		displayImage(mario_image, count, 200, count*deltaTime);
		for(const auto& pixel : newPixels)
		{
			drawCircle(pixel, 10, Pixel(255, 255, 255, 1), false);
		}
		if(mouse.mouse_held[0])
		{
			newPixels.emplace_back(mouse.mouse_x, mouse.mouse_y);
		}

		// Simple check to make sure the engine isn't chugging
		if(deltaTime > 1)
		{
			std::cout << "Engine is chugging" << std::endl;
		}
		++count;
	}
	virtual bool close() override
	{
		if(count > width)
		{
			return true;
		}
		return false;
	}
};

TEST_CASE("UI Test")
{
	TestEngine engine(500, 500, std::string("Title"));
	engine.launch();
}
