// STD
#include <string_view>// string_view
#include <memory.h>   // shared_ptr
#include <unistd.h>   // usleep
#include <filesystem>
#include <iostream>

// Source
#include "ruff/core/logger.hpp"
#include "ruff/geometry/point.hpp"
#include "ruff/ui/ui.hpp"

#define EMPTY ruff::ui::BLACK
#define SAND ruff::ui::DARK_YELLOW
#define WATER ruff::ui::DARK_BLUE

const constexpr static uint16_t width = 1000;
const constexpr static uint16_t height = 1500;

class Block;

class SandEngine : public ruff::ui::Engine
{
private:
	std::vector<std::shared_ptr<Block>> blocks{};

	int selected_type{ -1 };
	int brush_width{ 10 };

public:
	SandEngine(const uint16_t width, const uint16_t height)
	  : Engine(height, width)
	{
	}

	SandEngine(const SandEngine&) = delete;
	SandEngine(const SandEngine&&) = delete;
	SandEngine& operator=(const SandEngine&&) = delete;
	SandEngine& operator=(const SandEngine&) = delete;
	~SandEngine() override = default;

	bool query(const ruff::Point2D<uint16_t> pos)
	{
		return getPixel(pos) == EMPTY;
	};

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
				if(ROI[i + j] == EMPTY) { continue; }
				if(i * i + j * j <= brush_width * brush_width)
				{
					std::shared_ptr<Block> p = std::make_shared<T>(
					  ruff::Point2D<uint16_t>(mouse.mouse_x + j,
					                          mouse.mouse_y + i),
					  this);
					blocks.push_back(p);
				}
			}
		}
	}

	void onCreate() override;
	void onUpdate(double deltaTime) override;
	void onResize() override;
};

class Block
{
protected:
	SandEngine* engine;
	double v_angle{};

	ruff::Point2D<uint16_t> position{};
	ruff::Point2D<double> velocity{};
	ruff::Point2D<double> acceleration{};
	ruff::ui::Pixel color{};

public:
	[[nodiscard]] Block(ruff::Point2D<uint16_t> position,
	                    SandEngine* engine) noexcept
	  : engine(engine), position(position), velocity(1, 1),
	    acceleration(0, 1), color(EMPTY)
	{
	}

	virtual void update(double deltaTime)
	{
		(void)deltaTime;
		setVelocity(getVelocity() + getAcceleration());
		if(position.y >= width) { position.y = width - 1; }
		if(position.x >= height) { position.x = height - 1; }
	};
	virtual ~Block() = default;
	Block(const Block&) = default;
	Block(const Block&&) = delete;
	Block& operator=(const Block&) = default;
	Block& operator=(const Block&&) = delete;

	[[nodiscard]] auto getColor() const { return color; }
	void setPosition(ruff::Point2D<uint16_t> p) { this->position = p; }
	void setPosition(const double x, const double y)
	{
		this->position.x = x;
		this->position.y = y;
	};
	[[nodiscard]] ruff::Point2D<uint16_t> getPosition() const
	{
		return position;
	}

	void setVelocity(ruff::Point2D<double> v) { this->velocity = v; };
	void setVelocity(const double vx, const double vy)
	{
		this->velocity.x = vx;
		this->velocity.y = vy;
	};
	[[nodiscard]] ruff::Point2D<double> getVelocity() const
	{
		return velocity;
	};

	void setAcceleration(ruff::Point2D<double> a)
	{
		this->acceleration = a;
	};
	void setAcceleration(const double ax, const double ay)
	{
		this->acceleration.x = ax;
		this->acceleration.y = ay;
	};
	[[nodiscard]] ruff::Point2D<double> getAcceleration() const
	{
		return acceleration;
	};
};

class Sand : public Block
{
public:
	Sand(ruff::Point2D<uint16_t> position, SandEngine* engine) noexcept
	  : Block(position, engine)
	{
		color = SAND;
	}
	void update(double deltaTime) override
	{
		Block::update(deltaTime);
		const ruff::Point2D<uint16_t>& pos = getPosition();

		ruff::Point2D<uint16_t> down = pos;
		down.y += std::round(velocity.y);
		const auto line_down = engine->getLine(pos, down, 1);
		ruff::Point2D<uint16_t> left = pos;
		left.x -= std::round(velocity.x);
		left.y += std::round(velocity.y);
		const auto line_left = engine->getLine(pos, left, 1);
		ruff::Point2D<uint16_t> right = pos;
		right.x += std::round(velocity.x);
		right.y += std::round(velocity.y);
		const auto line_right = engine->getLine(pos, right, 1);

		for(auto down_it=line_down.rbegin(),left_it=line_left.rbegin(),right_it=line_right.rbegin();
		    down_it != line_down.rend()-1 || left_it != line_left.rend()-1 || right_it != line_right.rend()-1;)
		{
			if(down_it != line_down.rend())
			{
				if(engine->query(*down_it))
				{
					position = *down_it;
					return;
				}
				else
				{
					++down_it;
				}
			}
			if(left_it != line_left.rend())
			{
				if(engine->query(*left_it))
				{
					position = *left_it;
					return;
				}
				else
				{
					++left_it;
				}
			}
			if(right_it != line_right.rend())
			{
				if(engine->query(*right_it))
				{
					position = *right_it;
					return;
				}
				else
				{
					++right_it;
				}
			}
			velocity.y = 0;
		}
	}
};
class Water : public Block
{
private:
	signed char prevDir{ -1 };

public:
	Water(ruff::Point2D<uint16_t> position, SandEngine* engine) noexcept
	  : Block(position, engine)
	{
		color = WATER;
	}
	void update(double deltaTime) override { Block::update(deltaTime); }
};
