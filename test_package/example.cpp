#include <vector>
#include <iostream>

#include "ruff/geometry/point.hpp"
#include "ruff/ui/ui.hpp"

class TestEngine : public ruff::ui::Engine
{
public:
	TestEngine(const short int width,
	           const short int height,
	           const std::string& title)
	  : ruff::ui::Engine(height, width, title)
	{
	}
	virtual void onCreate() {}
	virtual void onUpdate(double deltaTime) override
	{
		clearScreen();
		drawLine(0, 0, 5, 5, ruff::ui::Pixel(128, 128, 128, 1));
	}
	virtual bool close() { return true; }
};
int main()
{
	ruff::Point2D<int> p1{ 5, 6 };
	ruff::Point2D<int> p2{ 1, 1 };
	std::cout << p1 + p2 << std::endl;
	TestEngine engine(500, 500, std::string("Title"));
	engine.launch();
}
