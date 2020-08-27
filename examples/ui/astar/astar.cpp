// STL Packages
#include <cmath>
#include <memory>

// Source
#include "ui/ui.hpp"
#include "ui/button.hpp"
#include "search.hpp"

using sint = short int;

class AStar : public ruff::ui::Engine
{
private:
	int startButton{};
	int blockWidthX{};
	int blockWidthY{};
	int gap{1};
	bool changed{true};
	std::vector<std::vector<bool>> map{};
	std::vector<std::vector<bool>> path{};

public:
	AStar(const sint width, const sint height, std::string title = "AStar Engine", int pixelRatio = 1) 
		: Engine(height, width, title, pixelRatio) {}

	AStar(const AStar& other) = delete;

	AStar& operator=(const AStar& other) = delete;

	virtual void onCreate() override 
	{
		// Initialize the found path, and the map as a 20x20 vector of booleans
		map = std::vector<std::vector<bool>>(20);
		path = std::vector<std::vector<bool>>(20);
		for(size_t i = 0; i < map.size(); ++i)
		{
			map[i] = std::vector<bool>(20);
			path[i] = std::vector<bool>(20);
			std::fill(map[i].begin(), map[i].end(), true);
			std::fill(path[i].begin(), path[i].end(), false);
		}

		// Find size of each grid tile
		blockWidthX = getWidth() / map.size();
		blockWidthY = getHeight() / map[0].size();

		// Add start button
		startButton = addButton(0, 0, blockWidthX, blockWidthY, ruff::ui::GREEN, pixelRatio,
				"../examples/ui/astar/DejaVuSans.ttf", "Start", 28);
	}
	virtual void onUpdate(double deltaTime) override 
	{
		(void)deltaTime;

		if(mouse.mouse_pressed[0])
		{
			/* If the mouse is pressed, figure out what grid tile the
			   cursor is in and then make it an obstacle */
			size_t x = mouse.mouse_x / blockWidthX;
			size_t y = mouse.mouse_y / blockWidthY;
			if(!(x == y && (x == 0 || x == map.size()-1))) // Don't modify start/end block
			{
				map[x][y] = !map[x][y];
				changed = true;
			}
		}

		clearScreen();

		if(buttons[startButton].get()->isPressed() && changed)
		{
			// Set start and end points to be true
			map[0][0] = true;
			map[map.size()-1][map[0].size()-1] = true;
			auto searcher = ruff::search::AStar(map);
			auto solution = searcher.getPath(0,0, map.size()-1, map[0].size()-1);
			changed = false;
			if(solution)
			{
				// Clear previous found path
				for(auto& p : path)
				{
					std::fill(p.begin(), p.end(), false);
				}

				// Get points and add them to the path
				auto points = solution.value();
				for(const auto& point : points)
				{
					path[point.x][point.y] = true;
				}
			}
			else
			{
				for(auto& p : path)
				{
					std::fill(p.begin(), p.end(), false);
				}
			}
		}

		for(size_t i = 0; i < map.size(); ++i)
		{
			for(size_t j = 0; j < map[i].size(); ++j)
			{
				if(!map[i][j])
				{
					// Draw obstacles
					drawSquare(i*blockWidthX + gap, j*blockWidthY + gap, i*blockWidthX+
							getWidth() / map.size() - gap, j*blockWidthY+ getHeight() /
							map[0].size() - gap, ruff::ui::RED, true);
				}
				else if(path[i][j] && buttons[startButton]->isPressed())
				{
					// Draw path
					drawSquare(i*blockWidthX + gap, j*blockWidthY + gap, i*blockWidthX+
							getWidth() / map.size() - gap, j*blockWidthY+ getHeight() /
							map[0].size() - gap, ruff::ui::YELLOW, true);
				}
			}
		}
	}
};

int main()
{
	// Create game engine and then run it
	AStar astarEngine(1000, 1200, "AStar", 2);
	astarEngine.launch();
	return 0;
}
