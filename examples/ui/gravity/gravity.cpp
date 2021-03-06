// STL Packages
#include <cmath>
#include <memory>

// Source
#include "ui/ui.hpp"

using sint = short int;

struct Body
{
	float px{ 0 }, py{ 0 };
	float vx{ 0 }, vy{ 0 };
	float ax{ 0 }, ay{ 0 };
	float radius{ 5 };
	float mass{ 50 };
	bool  moveable{ true };

	int id{};

	void print()
	{
		std::cout << "X: " << px << "\tY: " << py << std::endl;
		std::cout << "Radius: " << radius << std::endl;
	}
};

class Gravity : public ruff::ui::Engine
{
 private:
	std::vector<Body> planets{};
	bool              singleAnchor{ false };
	float             G = 6.673E-3;

	void addBody(float x, float y, float r = 10.0f)
	{
		Body b;
		b.px     = x;
		b.py     = y;
		b.vx     = 0;
		b.vy     = 0;
		b.ax     = 0;
		b.ay     = 0;
		b.radius = r;
		b.mass   = r * 10.0f;

		b.id = planets.size();
		planets.emplace_back(b);
	}

 public:
	Gravity(const sint width, const sint height, std::string title = "Gravity Engine", int pixelRatio = 1)
	  : Engine(height, width, title, pixelRatio) {}

	Gravity(const Gravity& other) = delete;

	Gravity& operator=(const Gravity& other) = delete;

	float distance(Body a, Body b)
	{
		return std::sqrt(std::pow(b.px - a.px, 2) + std::pow(b.py - a.py, 2));
	}

	virtual void onCreate() override
	{
		// Start with two anchors
		addBody(getWidth() * 0.35f, getHeight() * 0.5f, 2);
		planets[0].moveable = false;
		planets[0].mass     = 700;

		addBody(getWidth() * 0.65f, getHeight() * 0.5f, 2);
		planets[1].moveable = false;
		planets[1].mass     = 700;
	}
	virtual void onUpdate(double deltaTime) override
	{
		// Left click to add bodies
		if(mouse.mouse_pressed[0])
		{
			addBody(mouse.mouse_x, mouse.mouse_y, rand() % 10 + 10);
			planets[planets.size() - 1].vx = 2;
			mouse.mouse_pressed[0]         = false;
		}

		// Right click to change the number of anchors
		if(mouse.mouse_pressed[1])
		{
			planets.clear();
			if(!singleAnchor)
			{
				addBody(getWidth() * 0.5f, getHeight() * 0.5f, 2);
				planets[0].moveable = false;
				planets[0].mass     = 700;
				singleAnchor        = true;
			}
			else
			{
				addBody(getWidth() * 0.35f, getHeight() * 0.5f, 2);
				planets[0].moveable = false;
				planets[0].mass     = 700;

				addBody(getWidth() * 0.65f, getHeight() * 0.5f, 2);
				planets[1].moveable = false;
				planets[1].mass     = 700;
				singleAnchor        = false;
			}
			mouse.mouse_pressed[1] = false;
		}

		for(auto& ball : planets)
		{
			if(!ball.moveable)
				continue;

			// Update positions and velocities
			ball.px += ball.vx * deltaTime;
			ball.py += ball.vy * deltaTime;
			ball.vx += ball.ax * deltaTime;
			ball.vy += ball.ay * deltaTime;

			float total_fx = 0, total_fy = 0;
			for(auto& target : planets)
			{
				if(ball.id != target.id)
				{
					double dx   = ball.px - target.px;
					double dy   = ball.py - target.py;
					double dist = distance(ball, target);

					// Skip when the distance is very small so there isn't silly launching
					if(dist <= 0.5)
						continue;

					// Calculate orbital force and displacement between each body
					double force = G * ball.mass * target.mass / (dist /* * dist */);
					double angle = std::atan2(dy, dx);

					// Sum cumulative forces between all bodies
					total_fx += (std::cos(angle) * force);
					total_fy += (std::sin(angle) * force);
				}
			}
			ball.ax = -total_fx / ball.mass;
			ball.ay = -total_fy / ball.mass;
		}

		clearScreen();

		// Draw each body
		for(Body b : planets)
		{
			drawCircle(b.px, b.py, b.radius, ruff::ui::Pixel(b.mass, b.mass, b.mass), true);
		}
	}
};

int main()
{
	// Create game engine and then run it
	Gravity gravityEngine(1000, 1500, "Gravity", 2);
	gravityEngine.launch();
	return 0;
}
