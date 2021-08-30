// STL Packages
#include <cmath>
#include <memory>

// Source
#include "ruff/ui/ui.hpp"

using sint = short int;

struct Ball
{
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;
	float mass;

	int id;
};

class Circles : public ruff::ui::Engine
{
private:
	std::vector<Ball> balls{};
	Ball* selected{ nullptr };

	void addBall(const float x, const float y, const float r = 5.0f)
	{
		Ball b;
		b.px = x;
		b.py = y;
		b.vx = 0;
		b.vy = 0;
		b.ax = 0;
		b.ay = 0;
		b.radius = r;
		b.mass = r * 10.0f;

		b.id = balls.size();
		balls.emplace_back(b);
	}

public:
	Circles(const sint width, const sint height, 
			const std::string& title = "Circle Engine", 
			const int pixelRatio = 1)
	  : Engine(height, width, title, pixelRatio) {}

	Circles(const Circles& other) = delete;

	Circles& operator=(const Circles& other) = delete;

	virtual void onCreate() override
	{
		//addBall(width * .25f, height * 0.5f, fDefaultRad);
		//addBall(width * .75f, height * 0.5f, fDefaultRad);
		for(size_t i = 0; i < 30; ++i)
		{
			addBall(rand() % screenWidth, rand() % screenHeight, rand() % 16 + 2);
		}
	}
	virtual void onUpdate(double deltaTime) override
	{
		auto doCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2) {
			return std::abs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (r1 + r2) * (r1 + r2);
		};
		auto isPointInCircle = [](float x1, float y1, float r1, float px, float py) {
			return std::abs((x1 - px) * (x1 - px) + (y1 - py) * (y1 - py)) < (r1 * r1);
		};

		if(mouse.mouse_pressed[0] || mouse.mouse_pressed[1])
		{
			selected = nullptr;
			for(auto& ball : balls)
			{
				if(isPointInCircle(ball.px, ball.py, ball.radius, mouse.mouse_x, mouse.mouse_y))
				{
					selected = &ball;
					break;
				}
			}
		}

		if(mouse.mouse_held[0])
		{
			if(selected)
			{
				selected->px = mouse.mouse_x;
				selected->py = mouse.mouse_y;
			}
		}
		if(mouse.mouse_released[0])
		{
			selected = nullptr;
		}
		if(mouse.mouse_released[1])
		{
			if(selected)
			{
				selected->vx = 0.5f * ((selected->px) - mouse.mouse_x);
				selected->vy = 0.5f * ((selected->py) - mouse.mouse_y);
			}
			selected = nullptr;
		}

		std::vector<std::pair<Ball*, Ball*>> collidingBalls;

		for(auto& ball : balls)
		{
			// Apply drag
			ball.ax = -ball.vx * .008f;
			ball.ay = -ball.vy * .008f;

			ball.vx += ball.ax * (deltaTime);
			ball.vy += ball.ay * (deltaTime);
			ball.px += ball.vx * (deltaTime);
			ball.py += ball.vy * (deltaTime);

			if(ball.px < 0) ball.px += screenWidth;
			if(ball.py < 0) ball.py += screenHeight;
			if(ball.px > screenWidth) ball.px -= screenWidth;
			if(ball.py > screenHeight) ball.py -= screenHeight;

			if(std::abs(ball.vx * ball.vx + ball.vy * ball.vy) < 0.01f)
			{
				ball.vx = 0;
				ball.vy = 0;
			}
		}

		for(auto& ball : balls)
		{
			for(auto& target : balls)
			{
				if(ball.id != target.id)
				{
					if(doCirclesOverlap(ball.px, ball.py, ball.radius, target.px, target.py, target.radius))
					{
						collidingBalls.push_back({ &ball, &target });

						float distance = std::sqrt((ball.px - target.px) * (ball.px - target.px) + (ball.py - target.py) * (ball.py - target.py));

						float overlap = 0.5f * (distance - ball.radius - target.radius);

						// Displace current & target ball
						ball.px -= overlap * (ball.px - target.px) / distance;
						ball.py -= overlap * (ball.py - target.py) / distance;
						target.px += overlap * (ball.px - target.px) / distance;
						target.py += overlap * (ball.py - target.py) / distance;
					}
				}
			}
		}

		for(auto c : collidingBalls)
		{
			Ball* b1 = c.first;
			Ball* b2 = c.second;

			float distance = std::sqrt((b1->px - b2->px) * (b1->px - b2->px) + (b1->py - b2->py) * (b1->py - b2->py));

			// Normal
			float nx = (b2->px - b1->px) / distance;
			float ny = (b2->py - b1->py) / distance;

			// Tangent
			float tx = -ny;
			float ty = nx;

			// Dot Product Tangent
			float dpTan1 = b1->vx * tx + b1->vy * ty;
			float dpTan2 = b2->vx * tx + b2->vy * ty;

			// Dot Product Normal
			float dpNorm1 = b1->vx * nx + b1->vy * ny;
			float dpNorm2 = b2->vx * nx + b2->vy * ny;

			// Conservation of momentum in 1D
			float m1 = (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
			float m2 = (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b2->mass + b1->mass);

			b1->vx = tx * dpTan1 + nx * m1;
			b1->vy = ty * dpTan1 + ny * m1;
			b2->vx = tx * dpTan2 + nx * m2;
			b2->vy = ty * dpTan2 + ny * m2;
		}

		clearScreen();
		for(Ball b : balls)
		{
			drawCircle(b.px, b.py, b.radius, ruff::ui::WHITE);
		}

		for(auto c : collidingBalls)
		{
			drawLine(c.first->px, c.first->py, c.second->px, c.second->py, ruff::ui::RED);
		}
		if(selected)
		{
			drawLine(selected->px, selected->py, mouse.mouse_x, mouse.mouse_y);
		}
	}
};

int main()
{
	Circles circleEngine(1000, 1200, "Circles", 6);
	circleEngine.launch();
	return 0;
}
