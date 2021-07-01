// STD
#include <string_view> // string_view
#include <memory.h> // shared_ptr
#include <unistd.h> // usleep
#include <filesystem> 
#include <iostream>

// Source
#include "ui/ui.hpp"
#include "logger.hpp"

#define EMPTY ruff::ui::BLACK
#define SAND ruff::ui::DARK_YELLOW
#define WATER ruff::ui::DARK_BLUE

using sint = ruff::ui::sint;

const constexpr static sint width = 1000;
const constexpr static sint height = 1500;

class Block;

class SandEngine : public ruff::ui::Engine
{
private:
	std::vector<std::shared_ptr<Block>> blocks{};
	std::vector<int> types{};

	int selected_type{0};
	int brush_width = 3;

public:
	SandEngine(const sint width, const sint height, 
			const std::string& title = "Sand Engine", 
			const int pixelRatio = 6) 
		: Engine(height, width, title, pixelRatio) {}

	template<typename T> 
	void generate()
	{
		std::vector<ruff::ui::Pixel> ROI = getRegion(mouse.mouse_x - brush_width, mouse.mouse_y - brush_width, 
				mouse.mouse_x + brush_width, mouse.mouse_y + brush_width);
		for(int i = -brush_width; i < brush_width; ++i)
		{
			for(int j = -brush_width; j < brush_width; ++j)
			{
				if(ROI[i + j] == EMPTY)
				{
					continue;
				}
				if(i*i + j*j <= brush_width*brush_width)
				{
					std::shared_ptr<Block> p = 
						std::make_shared<T>(ruff::Point2D<sint>(mouse.mouse_x+j, mouse.mouse_y+i));
					blocks.push_back(p);
				}
			}
		}
	}


	~SandEngine() override = default;

	SandEngine(const SandEngine&) = delete;
	SandEngine(const SandEngine&&) = delete;
	SandEngine& operator=(const SandEngine&&) = delete;
	SandEngine& operator=(const SandEngine&) = delete;

	void onCreate() override;
	void onUpdate(double deltaTime) override;

};



class Block 
{
protected:
	ruff::ui::Pixel color{};
	ruff::Point2D<sint> position{};

public:
	[[nodiscard]] Block(ruff::Point2D<sint> position) noexcept : position(position)
	{
		color = EMPTY;
	}

	virtual void update(double deltaTime, SandEngine* engine) {(void)deltaTime; (void)engine; };
	virtual ~Block() = default;
	Block(const Block&) = default;
	Block(const Block&&) = delete;
	Block& operator=(const Block&) = default;
	Block& operator=(const Block&&) = delete;

	[[nodiscard]] auto getPosition() const { return position; }
	[[nodiscard]] auto getColor() const { return color; }

};

class Sand : public Block
{
public:
	Sand(ruff::Point2D<sint> position) noexcept : Block(position) 
	{
		color = SAND;
	}
	void update(double deltaTime, SandEngine* engine) override
	{
		(void)deltaTime;
		if(position.y >= height || position.y <= 0) // Top or bottom of screen
		{
			return;
		}
		if(position.x >= width || position.x <= 0)
		{
			return;
		}
		std::vector<ruff::ui::Pixel> ROI = engine->getRegion(position.x-1, position.y+1, 
				position.x+1, position.y+1);

		if(ROI[1] == EMPTY)
		{
			position.y++;
		}
		else if(ROI[0] == EMPTY)
		{
			position.x--;
			position.y++;
		}
		else if(ROI[2] == EMPTY)
		{
			position.x++;
			position.y++;
		}
		
	}
};
class Water : public Block
{
private:
	signed char prevDir{-1}; 
public:
	Water(ruff::Point2D<sint> position) noexcept : Block(position) 
	{
		color = WATER;
	}
	void update(double deltaTime, SandEngine* engine) override
	{
		(void)deltaTime;
		if(position.y >= height || position.y <= 0) // Top or bottom of screen
		{
			return;
		}
		if(position.x >= width || position.x <= 0)
		{
			return;
		}
		// [ 0:left 1:block 2:right 
		// 3:bottom-left 4:down 5:bottom-right]
		std::vector<ruff::ui::Pixel> ROI = engine->getRegion(position.x-1, position.y, 
				position.x+1, position.y+1);

		if(ROI[4] == EMPTY) // Down
		{
			position.y++;
		}
		else if(ROI[3] == EMPTY) // Down left
		{
			position.x--;
			position.y++;
		}
		else if(ROI[5] == EMPTY) // Down right
		{
			position.x++;
			position.y++;
		}
		else if(ROI[0] == EMPTY && ROI[2] == EMPTY)
		{
			position.x += prevDir;
		}
		else if(ROI[0] == EMPTY) // Left
		{
			position.x--;
			prevDir = -1;
		}
		else if(ROI[2] == EMPTY) // Right
		{
			position.x++;
			prevDir = 1;
		}
		
	}
};
