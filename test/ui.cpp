#include "doctest.h"
#include <iostream>
#include <chrono>
#include <thread>

#include "ui/ui.hpp"
using namespace ruff::ui;

class TestEngine : public Engine
{
public:
	TestEngine(const sint width, const sint height, std::string title = "Window") : Engine(height, width, title) {}
	int count{};
	virtual void onCreate() override
	{
		count = 0;
	}
	virtual void onUpdate() override
	{
		clearScreen();
		drawLine(0,0,count,count, Pixel(255,255,255,1));
		drawCircle(count, count, 50, Pixel(255,255,255,1), true);
		drawCircle(mouse_x, mouse_y, 10, Pixel(255,255,255,1), true);
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

