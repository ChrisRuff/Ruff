#include "doctest.h"
#include <iostream>
#include <chrono>
#include <thread>

#include "ui/ui.hpp"
using namespace ruff::ui;

class TestEngine : public Engine
{
public:
	TestEngine(const sint width, const sint height, std::string title = "Ruff Pixel Engine") 
		: Engine(height, width, title) {}
	int count{};
	std::vector<ruff::Point2D<short int>> newPixels;
	virtual void onCreate() override
	{
		count = 0;
	}
	virtual void onUpdate(double deltaTime) override
	{
		clearScreen();
		drawLine(0,0,count,count, Pixel(255,255,255,1));
		drawCircle(count, count, 50, Pixel(255,255,255,1), true);
		drawCircle(mouse.mouse_x, mouse.mouse_y, 10, Pixel(255,255,255,1), true);
		for(const auto& pixel : newPixels)
		{
			drawCircle(pixel, 10, Pixel(255,255,255,1), false);
		}
		if(mouse.mouse_held[0])
		{
			newPixels.emplace_back(mouse.mouse_x, mouse.mouse_y);
		}
		++count;
	}
	virtual bool close() override
	{
		if(count > width)
			return true;
		return false;
	}
};

TEST_CASE("UI Test")
{
	TestEngine engine(500,500,std::string("Title"));
	engine.launch();
}

