#include "ruff/ui/ui.hpp"

namespace ruff
{
namespace ui
{
	Engine::Engine(const uint16_t width,
	               const uint16_t height,
	               const Pixel p)
	  : screen(std::make_unique<Window>(width, height, p))
	{
		keys.fill(false);
	}

	double Engine::iterate()
	{
		SDL_Event event;
		if(mouse.mouse_released[0]) { mouse.mouse_released[0] = false; }
		if(mouse.mouse_released[1]) { mouse.mouse_released[1] = false; }
		if(mouse.mouse_released[2]) { mouse.mouse_released[2] = false; }
		if(mouse.mouse_pressed[0]) { mouse.mouse_pressed[0] = false; }
		if(mouse.mouse_pressed[1]) { mouse.mouse_pressed[1] = false; }
		if(mouse.mouse_pressed[2]) { mouse.mouse_pressed[2] = false; }
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					onResize();
					screen->resize(
						{ static_cast<uint16_t>(event.window.data1),
							static_cast<uint16_t>(event.window.data2) });
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					mouse.mouse_held[0] = false;
					mouse.mouse_released[0] = true;
				}
				else if(event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.mouse_held[1] = false;
					mouse.mouse_released[1] = true;
				}
				else if(event.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse.mouse_held[2] = false;
					mouse.mouse_released[2] = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.mouse_pressed[1] = true;
					mouse.mouse_held[1] = true;
				}
				else if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					mouse.mouse_pressed[0] = true;
					mouse.mouse_held[0] = true;
				}
				else if(event.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse.mouse_pressed[2] = true;
					mouse.mouse_held[2] = true;
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouse.mouse_x, &mouse.mouse_y);
				mouse.mouse_x /= screen->getRatio();
				mouse.mouse_y /= screen->getRatio();
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) { running = false; }
				else if(event.key.keysym.sym
								>= static_cast<int>(keys.size()))
				{
					break;
				}
				keys[event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym >= static_cast<int>(keys.size()))
				{
					break;
				}
				keys[event.key.keysym.sym] = false;
				break;
			}
		}

		double delta = screen->renderer->getDeltaTime();
		onUpdate(delta);
		for(auto& button : buttons) { drawButton(button.get()); }
		if(mouse.mouse_pressed[0])
		{
			for(auto& button_ptr : buttons)
			{
				const Point2D<uint16_t> size = button_ptr->dims();
				const Point2D<uint16_t> pos = button_ptr->xy();
				if(mouse.mouse_x - pos.x < size.x
					 && mouse.mouse_x - pos.x > 0
					 && mouse.mouse_y - pos.y < size.y
					 && mouse.mouse_y - pos.y > 0) [[likely]]
				{
					button_ptr->press();
				}
			}
		}
		screen->push_to_screen();

		if(close()) { running = false; }
		return delta;
	}
	void Engine::launch()
	{
		onCreate();
		double frame_timer{0};
		size_t frame_count{0};
		while(running)
		{
			double delta = iterate();
			frame_timer += delta;
			++frame_count;
			if(frame_timer >= 1)
			{
				frame_timer -= 1;
				screen->setTitle(std::string("Ruff UI Engine - FPS:" + std::to_string(frame_count)));
				frame_count = 0;
			}
		}
	}

	void Engine::displayImage(const ruff::ui::Image& img,
	                          const uint16_t x,
	                          const uint16_t y,
	                          const double rotation)
	{
		Image image;
		if(rotation != 0) { image = img.rotate(rotation); }
		else
		{
			image = img;
		}
		for(uint16_t i = x; i < image.width() + x && i < getWidth(); ++i)
		{
			for(uint16_t j = y; j < image.height() + y && j < getHeight();
			    ++j)
			{
				const Pixel new_p = image.get(i - x, j - y);
				draw(i, j, new_p);
			}
		}
	}

	void Engine::clearScreen() { screen->clear(); }
	void Engine::draw(const uint16_t x, const uint16_t y, Pixel color)
	{
		screen->draw(x, y, color);
	}
	void Engine::draw(const Point2D<uint16_t>& p, const Pixel& color)
	{
		draw(p.x, p.y, color);
	}

	std::vector<Point2D<uint16_t>> Engine::getLine(const uint16_t x1,
	                                               const uint16_t y1,
	                                               const uint16_t x2,
	                                               const uint16_t y2,
	                                               const int line_width)
	{
		std::vector<Point2D<uint16_t>> points{};
		int16_t dx = x2 - x1;
		int16_t dy = y2 - y1;

		// Vertical
		if(dx == 0)
		{
			if(y2 < y1) { return getLine(x2, y2, y1, y1, line_width); }
			else
			{
				points.reserve(y2 - y1);
				for(uint16_t y = y1; y <= y2; ++y)
				{
					points.emplace_back( x1, y );
				}
			}
		}
		// Horizontal
		else if(dy == 0)
		{
			points.reserve(x2 - x1);
			if(x2 < x1) { return getLine(x2, y2, x1, y1, line_width); }
			else
			{
				for(uint16_t x = x1; x <= x2; ++x)
				{
					points.emplace_back( x, y1 );
				}
			}
		}
		// at an angle not divisible by 90
		else
		{
			int16_t dx1 = std::abs(dx);
			int16_t dy1 = std::abs(dy);
			int16_t px = 2 * dy1 - dx1;
			int16_t py = 2 * dx1 - dy1;
			uint16_t x, y, xe, ye;
			if(dy1 <= dx1)
			{
				if(dx >= 0)
				{
					x = x1;
					y = y1;
					xe = x2;
				}
				else
				{
					x = x2;
					y = y2;
					xe = x1;
				}

				points.emplace_back( x, y );
				points.reserve(xe);
				for(uint16_t i = 0; x < xe; ++i)
				{
					++x;
					if(px < 0) { px = px + 2 * dy1; }
					else
					{
						if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
							++y;
						else
							--y;

						px += 2 * (dy1 - dx1);
					}
					points.emplace_back(x, y);
				}
			}
			else
			{
				if(dy >= 0)
				{
					x = x1;
					y = y1;
					ye = y2;
				}
				else
				{
					x = x2;
					y = y2;
					ye = y1;
				}

				points.emplace_back( x, y );

				for(uint16_t i = 0; y < ye; ++i)
				{
					++y;
					if(py <= 0) { py += 2 * dx1; }
					else
					{
						if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
							++x;
						else
							--x;
						py += 2 * (dx1 - dy1);
					}
					points.emplace_back( x, y );
				}
			}
		}
		return points;
	}
	std::vector<Point2D<uint16_t>>
	  Engine::getLine(const Point2D<uint16_t>& p1,
	                  const Point2D<uint16_t>& p2,
	                  const int line_width)
	{
		return Engine::getLine(p1.x, p1.y, p2.x, p2.y, line_width);
	}
	void Engine::drawLine(const uint16_t x1,
	                      const uint16_t y1,
	                      const uint16_t x2,
	                      const uint16_t y2,
	                      const Pixel& color,
	                      const int line_width)
	{
		const auto pts = Engine::getLine(x1, y1, x2, y2, line_width);
		for(const auto& pt : pts) { draw(pt, color); }
	}
	void Engine::drawLine(const Point2D<uint16_t>& p1,
	                      const Point2D<uint16_t>& p2,
	                      const Pixel& color,
	                      const int line_width)
	{
		drawLine(p1.x, p1.y, p2.x, p2.y, color, line_width);
	}

	void Engine::drawSquare(const uint16_t leftX,
	                        const uint16_t leftY,
	                        const uint16_t rightX,
	                        const uint16_t rightY,
	                        const Pixel& color,
	                        const bool fill)
	{
		if(leftX > rightX)
			drawSquare(rightX, rightY, leftX, leftY, color, fill);

		if(fill)
		{
			for(uint16_t i = leftX; i < rightX; ++i)
			{
				drawLine(i, leftY, i, rightY, color, 1);
			}
		}
		else
		{
			drawLine(leftX, leftY, rightX, leftY);
			drawLine(leftX, leftY, leftX, rightY);
			drawLine(leftX, rightY, rightX, rightY);
			drawLine(rightX, leftY, rightX, rightY);
		}
	}

	void Engine::drawSquare(const Point2D<uint16_t>& left,
	                        const Point2D<uint16_t>& right,
	                        const Pixel& color,
	                        const bool fill)
	{
		drawSquare(left.x, left.y, right.x, right.y, color, fill);
	}


	void Engine::drawCircle(const uint16_t centerX,
	                        const uint16_t centerY,
	                        const uint16_t radius,
	                        const Pixel& color,
	                        const bool fill)
	{
		// Equation of a circle
		auto y = [centerX, radius](uint16_t x) {
			return std::sqrt((radius * radius)
			                 - (x - centerX) * (x - centerX));
		};
		if(!fill)
		{
			for(uint16_t x = centerX - radius; x <= centerX + radius; ++x)
			{
				draw(x, y(x) + centerY, color);
				draw(x, -y(x) + centerY, color);
			}
		}
		else
		{
			for(int x = centerX - radius; x <= centerX + radius; ++x)
			{
				if(x < 0)
				{
					continue;
				}
				double lowerbound = centerY - y(x);
				double upperbound = centerY + y(x);

				drawLine(x, lowerbound < 0 ? 0 : lowerbound,
				         x, upperbound,
				         color);
			}
		}
	}
	void Engine::drawCircle(const Point2D<uint16_t>& center,
	                        const uint16_t radius,
	                        const Pixel& color,
	                        const bool fill)
	{
		drawCircle(center.x, center.y, radius, color, fill);
	}

	void Engine::drawButton(Button* button)
	{
		const Point2D<uint16_t> size = button->dims();
		const Point2D<uint16_t> pos = button->xy();
		drawSquare(pos.x,
		           pos.y,
		           pos.x + size.x,
		           pos.y + size.y,
		           button->getColor(),
		           true);

		auto message = std::unique_ptr<SDL_Texture, SDLDestroyer>(
		  SDL_CreateTextureFromSurface(screen->renderer->getRenderer(),
		                               nullptr));

		SDL_Rect message_rect;
		message_rect.x = pos.x;
		message_rect.y = pos.y;
		message_rect.w = size.x;
		message_rect.h = size.y;

		SDL_RenderCopy(screen->renderer->getRenderer(),
		               message.get(),
		               NULL,
		               &message_rect);
	}
	Button* Engine::addButton(const Point2D<uint16_t> xy,
	                          const Point2D<uint16_t> size)
	{
		size_t idx = buttons.size();
		buttons.push_back(
		  std::make_unique<Button>(Point2D<uint16_t>(xy.x, xy.y),
		                           Point2D<uint16_t>(size.x, size.y)));
		return buttons[idx].get();
	}
	Pixel Engine::getPixel(uint16_t x, uint16_t y) const
	{
		return screen->get(x, y);
	}
	Pixel Engine::getPixel(Point2D<uint16_t> p) const
	{
		return getPixel(p.x, p.y);
	}
	std::vector<Pixel> Engine::getRegion(uint16_t x1,
	                                     uint16_t y1,
	                                     uint16_t x2,
	                                     uint16_t y2)
	{
		if(x1 > x2) return getRegion(x2, y1, x1, y2);
		if(y1 > y2) return getRegion(x1, y2, y1, x2);

		std::vector<Pixel> region{};
		for(uint16_t y = y1; y <= y2; ++y)
		{
			for(uint16_t x = x1; x <= x2; ++x)
			{
				if(x >= getWidth() || y >= getHeight())
				{
					region.push_back(ruff::ui::BLANK);
				}
				else
				{
					region.emplace_back(screen->get(x, y));
				}
			}
		}
		return region;
	}

	std::vector<Pixel> Engine::getRegion(Point2D<uint16_t> p1,
	                                     Point2D<uint16_t> p2)
	{
		return getRegion(p1.x, p1.y, p2.x, p2.y);
	}

	[[nodiscard]] bool Engine::onButton(const MouseState& mouse) const
	{
		for(const auto& b : buttons)
		{
			const Point2D<uint16_t> size = b->dims();
			const Point2D<uint16_t> pos = b->xy();
			if(std::abs(pos.x - mouse.mouse_x) < size.x
			   && std::abs(pos.y - mouse.mouse_y) < size.y)
			{
				return true;
			}
		}
		return false;
	}

	Pixel getRandColor()
	{
		return Pixel(
		  std::rand() % 256, std::rand() % 256, std::rand() % 256);
	}

};// namespace ui
};// namespace ruff
