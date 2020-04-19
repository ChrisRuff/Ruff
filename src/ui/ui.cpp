#include "ui/ui.hpp"

namespace ruff
{
	namespace ui
	{
		using sint = short int;
		Engine::Engine(const sint width, const sint height, std::string title, int pixelRatio) : 
			width(width), height(height), title(title), screenWidth(width/pixelRatio), screenHeight(height/pixelRatio),
			pixelRatio(pixelRatio)
		{    
			keys.fill(false);
		}
		void Engine::launch()
		{
			if (SDL_Init(SDL_INIT_VIDEO) != 0)
			{
					printf("Error: %s\n", SDL_GetError());
			}
			else
			{
				window = std::unique_ptr<SDL_Window, SDLDestroyer>(SDL_CreateWindow(title.c_str(), 
						SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
						width, height, SDL_WINDOW_OPENGL));
				renderer = std::unique_ptr<SDL_Renderer, SDLDestroyer>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
				texture = std::unique_ptr<SDL_Texture, SDLDestroyer>(SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ARGB8888,
						SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight));
				pixels.reserve(screenWidth * screenHeight * 4);
				std::fill(pixels.begin(), pixels.end(), 0);
				onCreate();

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
				SDL_SetWindowTitle(window.get(), title.c_str());
				gl_context = SDL_GL_CreateContext(window.get());

				SDL_Event event;
				bool running = true;

				double now = SDL_GetPerformanceCounter();
				double last = 0.0f;
				double deltaTime = 0;
				while(running)
				{
					last = now;
					if(mouse.mouse_released[0])
					{
						mouse.mouse_released[0] = false;
					}
					if(mouse.mouse_released[1])
					{
						mouse.mouse_released[1] = false;
					}
					while(SDL_PollEvent(&event))
					{
						switch(event.type)
						{
							case SDL_QUIT:
								running = false;
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
								break;
							case SDL_MOUSEBUTTONDOWN: 
								if(event.button.button == 3)//SDL_BUTTON(SDL_BUTTON_RIGHT))
								{
									if(mouse.mouse_pressed[1])
										mouse.mouse_pressed[1] = false;
									else
										mouse.mouse_pressed[1] = true;
									mouse.mouse_held[1] = true;
								}
								else if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
								{
									if(mouse.mouse_pressed[0])
										mouse.mouse_pressed[0] = false;
									else
										mouse.mouse_pressed[0] = true;
									mouse.mouse_held[0] = true;
								}
								break;
							case SDL_MOUSEMOTION:
								SDL_GetMouseState(&mouse.mouse_x, &mouse.mouse_y);
								mouse.mouse_x /= pixelRatio;
								mouse.mouse_y /= pixelRatio;
								break;
							case SDL_KEYDOWN:
								if(event.key.keysym.sym == SDLK_ESCAPE)
								{
									running = false;
								}
								keys[event.key.keysym.sym] = true;
								break;
							case SDL_KEYUP:
								keys[event.key.keysym.sym] = false;
								break;
						}
					}
					now = SDL_GetPerformanceCounter();
					deltaTime = static_cast<double>(now - last) * 100 / static_cast<double>(SDL_GetPerformanceFrequency());
					onUpdate(deltaTime);
					SDL_UpdateTexture(texture.get(), NULL, &pixels[0], screenWidth * 4);
					SDL_RenderCopy( renderer.get(), texture.get(), NULL, NULL );
					SDL_RenderPresent( renderer.get() );
					if(close())
					{
						running = false;
					}
				}
				SDL_DestroyRenderer(renderer.get());
				SDL_DestroyWindow(window.get());
				SDL_Quit();
			}
		}
		void Engine::clearScreen(Pixel color)
		{
			// Calculate all possible pixels
			size_t size = (screenWidth*screenHeight)*4; 

			// Replace the pixel at each location with the given color
			for(size_t i = 0; i < size; i+=3)
			{
				pixels[i] = color[0];
				pixels[i+1] = color[1];
				pixels[i+2] = color[2];
				pixels[i+3] = color[3];
			}
		}
		void Engine::draw(const sint x, const sint y, const Pixel& color)
		{
			if(x < 0 || y < 0 || x >= screenWidth || y >= screenHeight)
				return;
			const unsigned int offset = (screenWidth * 4 * y) + x * 4;
			pixels[offset]     = color.r;
			pixels[offset + 1] = color.g;
			pixels[offset + 2] = color.b;
			pixels[offset + 3] = color.a;
		}
		void Engine::draw(const Point2D<sint>& p, const Pixel& color) { draw(p.x, p.y, color); }

		void Engine::drawLine(const sint x1, const sint y1, const sint x2, const sint y2, 
				const Pixel& color, const int line_width)
		{
			sint dx = x2-x1;
			sint dy = y2-y1;

			//Vertical
			if(dx == 0)
			{
				if(y2 < y1) drawLine(x2, y2, y1, y1, color, line_width);
				else
				{
					for(sint y = y1; y <= y2; ++y)
					{
						draw(x1, y, color);
					}
				}
				return;
			}
			// Horizontal
			else if(dy == 0)
			{
				if(x2 < x1) drawLine(x2, y2, x1, y1, color, line_width);
				else
				{
					for(sint x = x1; x <= x2; ++x)
					{
						draw(x, y1, color);
					}
				}
				return;
			}
			// at an angle not divisible by 90
			else
			{
				sint dx1 = std::abs(dx);
				sint dy1 = std::abs(dy);
				sint px = 2 * dy1 - dx1;
				sint py = 2 * dx1 - dy1;
				sint x, y, xe, ye;
				if(dy1 <= dx1)
				{
					if(dx >= 0)
					{
						x = x1; y = y1; xe = x2;
					}
					else
					{
						x = x2; y = y2; xe = x1;
					}

					draw(x, y, color);
					for(sint i = 0; x < xe; ++i)
					{
						++x;
						if(px < 0)
						{
							px = px + 2 * dy1;
						}
						else
						{
							if((dx < 0 && dy < 0) || (dx>0 && dy>0)) ++y;
							else --y;

							px += 2 * (dy1 - dx1);
						}
						draw(x, y, color);
					}
				}
				else
				{
					if(dy >= 0)
					{
						x = x1; y = y1; ye = y2;
					}
					else
					{
						x = x2; y = y2; ye = y1;
					}

					draw(x, y, color);

					for(sint i = 0; y < ye; ++i)
					{
						++y;
						if(py <= 0)
						{
							py+= 2 * dx1;
						}
						else
						{
							if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ++x;
							else --x;
							py += 2 * (dx1 - dy1);
						}
						draw(x, y, color);
					}
				}
			}
		}
		void Engine::drawLine(const Point2D<sint>& p1, const Point2D<sint>& p2, 
				const Pixel& color) 
		{ 
			drawLine(p1.x, p1.y, p2.x, p2.y, color);
		}
		void Engine::drawCircle(const sint centerX, const sint centerY, 
				const sint radius, const Pixel& color, const bool fill)
		{
			// Equation of a circle
			auto y = [centerX, radius](sint x)
			{
				return std::sqrt((radius * radius) - (x-centerX) * (x-centerX));
			};
			if(!fill)
			{
				for(sint x = centerX - radius; x <= centerX + radius; ++x)
				{
					draw(x, y(x) + centerY, color);
					draw(x, -y(x) + centerY, color);
				}
			}
			else
			{
				for(sint x = centerX - radius; x <= centerX + radius; ++x)
				{
					drawLine(x, -y(x) + centerY, x, y(x) + centerY, color);
				}
			}

		}
		void Engine::drawCircle(const Point2D<sint>& center, const sint radius, 
				const Pixel& color, const bool fill) 
		{ drawCircle(center.x, center.y, radius, color, fill); } 

	};
};
