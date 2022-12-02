// STD
#include <string_view>// string_view
#include <memory.h>   // shared_ptr
#include <unistd.h>   // usleep
#include <filesystem>
#include <iostream>

// Source
#include <ruff/core/structs/point.hpp>
#include <ruff/core/logger.hpp>
#include <ruff/ui/ui.hpp>

#define EMPTY ruff::imgproc::BLACK
#define SAND ruff::imgproc::DARK_YELLOW
#define WATER ruff::imgproc::DARK_BLUE

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

	bool query(const ruff::Point2D<uint16_t>& pos)
	{
		return getPixel(pos) == EMPTY;
	};

	template<typename T>
	void generate()
	{
		std::vector<ruff::imgproc::Pixel> ROI =
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
	ruff::imgproc::Pixel color{};

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
	  auto ToSINT = [](int in){ return static_cast<uint16_t>(in);};
	  const ruff::Point2D<uint16_t> down =      { ToSINT(pos.x),     ToSINT(pos.y + 1) };
	  const ruff::Point2D<uint16_t> left =      { ToSINT(pos.x - 1), ToSINT(pos.y + 1) };
	  const ruff::Point2D<uint16_t> right =     { ToSINT(pos.x + 1), ToSINT(pos.y + 1) };

		if(engine->query(down))
		{
			position = down;
		}
		else if(engine->query(left))
		{
			position = left;
		}
		else if(engine->query(right))
		{
			position = right;
		}
	}
};
class Water : public Block
{
private:
	enum class DIR
	{
		LEFT,
		RIGHT
	};
	DIR travel{ DIR::LEFT };

public:
	Water(ruff::Point2D<uint16_t> position, SandEngine* engine) noexcept
	  : Block(position, engine)
	{
		color = WATER;
	}
	void update(double deltaTime) override
	{
		Block::update(deltaTime);
		auto ToSINT = [](int in){ return static_cast<uint16_t>(in);};
		const ruff::Point2D<uint16_t>& pos = getPosition();
 		const ruff::Point2D<uint16_t> down =      { ToSINT(pos.x),     ToSINT(pos.y + 1) };
		const ruff::Point2D<uint16_t> left =      { ToSINT(pos.x - 1), ToSINT(pos.y + 1) };
		const ruff::Point2D<uint16_t> right =     { ToSINT(pos.x + 1), ToSINT(pos.y + 1) };
		const ruff::Point2D<uint16_t> mid_left =  { ToSINT(pos.x - 1), ToSINT(pos.y) };
		const ruff::Point2D<uint16_t> mid_right = { ToSINT(pos.x + 1), ToSINT(pos.y) };

		if(engine->query(down))
		{
			position = down;
		}
		else if(engine->query(left))
		{
			position = left;
		}
		else if(engine->query(right))
		{
			position = right;
			travel = DIR::RIGHT;
		}
		else if(travel == DIR::LEFT)
		{
			if(engine->query(mid_left))
			{
				position = mid_left;
			}
			else if(engine->query(mid_right))
			{
				position = mid_right;
				travel = DIR::RIGHT;
			}
		}
		else if(travel == DIR::RIGHT)
		{
			if(engine->query(mid_right))
			{
				position = mid_right;
			}
			else if(engine->query(mid_left))
			{
				position = mid_left;
				travel = DIR::LEFT;
			}
		}
	}
};
