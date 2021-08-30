// STD
#include <string_view> // string_view
#include <memory.h> // shared_ptr
#include <unistd.h> // usleep
#include <filesystem> 
#include <iostream>

// Source
#include "ruff/ui/ui.hpp"
#include "ruff/core/logger.hpp"
#include "ruff/geometry/point.hpp"

#define EMPTY ruff::ui::BLACK
#define SAND ruff::ui::DARK_YELLOW
#define WATER ruff::ui::DARK_BLUE

using sint = ruff::ui::sint;

const constexpr static sint width = 1000;
const constexpr static sint height = 1500;

const constexpr static sint pr = 6;
const constexpr static sint w = width / pr;
const constexpr static sint h = height / pr;

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
			const int pixelRatio = pr) 
		: Engine(height, width, title, pixelRatio) {}

	template<typename T> 
	void generate()
	{
		std::vector<ruff::ui::Pixel> ROI = 
			getRegion(mouse.mouse_x - brush_width, 
					mouse.mouse_y - brush_width, 
					mouse.mouse_x + brush_width, 
					mouse.mouse_y + brush_width);
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
	double v_angle{};

	ruff::Point2D<sint> position{};
	ruff::Point2D<double> velocity{};
	ruff::Point2D<double> acceleration{};
	ruff::ui::Pixel color{};

public:
	[[nodiscard]] Block(ruff::Point2D<sint> position) noexcept : 
		position(position), velocity(0,0), acceleration(0,1), color(EMPTY)
	{
	}

	virtual void update(double deltaTime) 
	{
		if(position.y < 0)
		{
			position.y = 0;
		}
		else if(position.y >= w)
		{
			position.y = w-1;
		}

		if(position.x < 0)
		{
			position.x = 0;
		}
		else if(position.x >= h)
		{
			position.x = h-1;
		}
	};
	virtual constexpr std::array<bool, 9> collision()
	{
		return 
		{false, false, false,
		 false, false, false, 
		 false ,false, false };
	};
	virtual ~Block() = default;
	Block(const Block&) = default;
	Block(const Block&&) = delete;
	Block& operator=(const Block&) = default;
	Block& operator=(const Block&&) = delete;

	[[nodiscard]] auto getColor() const { return color; }
	void setPosition(ruff::Point2D<sint> p) { this->position = p; }
	void setPosition(const double x, const double y) 
	{ 
		this->position.x = x; 
		this->position.y = y; 
	};
	[[nodiscard]] ruff::Point2D<sint> getPosition() const { return position; }

	void setVelocity(ruff::Point2D<double> v) { this->velocity = v; };
	void setVelocity(const double vx, const double vy) 
	{ 
		this->velocity.x = vx; 
		this->velocity.y = vy; 
	};
	[[nodiscard]] ruff::Point2D<double> getVelocity() const { return velocity; };

	void setAcceleration(ruff::Point2D<double> a) { this->acceleration = a; };
	void setAcceleration(const double ax, const double ay) 
	{ 
		this->acceleration.x = ax; 
		this->acceleration.y = ay; 
	};
	[[nodiscard]] ruff::Point2D<double> getAcceleration() const { return acceleration; };
};

class Sand : public Block
{
public:
	Sand(ruff::Point2D<sint> position) noexcept : Block(position) 
	{
		color = SAND;
	}
	void update(double deltaTime) override
	{
		Block::update(deltaTime);
	}
	constexpr std::array<bool, 9> collision() override
	{
		return 
		{false, false, false,
		 false, true , false, 
		 true , true , true };
	};
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
	void update(double deltaTime) override
	{
		Block::update(deltaTime);
	}
	constexpr std::array<bool, 9> collision() override
	{
		return 
		{false, false, false,
		 true , true , true, 
		 true , true , true };
	};
};
