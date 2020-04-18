#include "ui/ui.hpp"

namespace ruff
{
	namespace ui
	{
		using sint = short int;
		Engine::Engine(const sint width, const sint height, std::string title) : width(width), height(height), title(title)
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
						SDL_TEXTUREACCESS_STREAMING, width, height));
				pixels.reserve(height * width * 4);
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

				while(running)
				{
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
									mouse_buttons[0] = false;
								}
								else if(event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
								{
									mouse_buttons[1] = false;
								}
								break;
							case SDL_MOUSEBUTTONDOWN: 
								if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
								{
									mouse_buttons[0] = true;
								}
								else if(event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
								{
									mouse_buttons[1] = true;
								}
								break;
							case SDL_MOUSEMOTION:
								SDL_GetMouseState(&mouse_x, &mouse_y);
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
					onUpdate();
					SDL_UpdateTexture(texture.get(), NULL, &pixels[0], width * 4);
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
			size_t size = width*height*4; 

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
			if(x < 0 || y < 0 || x > width || y > height)
				return;
			const unsigned int offset = (width * 4 * y) + x * 4;
			pixels[offset]     = color.r;
			pixels[offset + 1] = color.g;
			pixels[offset + 2] = color.b;
			pixels[offset + 3] = color.a;
		}
		void Engine::draw(const Point2D<sint>& p, const Pixel& color) { draw(p.x, p.y, color); }

		void Engine::drawLine(const sint x1, const sint y1, const sint x2, const sint y2, 
				const Pixel& color, const int line_width)
		{

			// Deal with same x value seperately because the slope would be infinity
			if(x2 == x1)
			{
				sint max, min;
				if(y2 > y1)
				{
					max = y2;
					min = y1;
				}
				else
				{
					max = y1;
					min = y2;
				}

				for(sint i = min; i < max; ++i)
				{
					draw(x1, i, color);
				}
				return;
			}
			if(x1 > x2)
			{
				drawLine(x2, y2, x1, y1, color);
				return;
			}

			// Solve the equation of the line
			int slope = (y2 - y1) / (x2 - x1);
			auto y = \
				[slope, x1, y1](sint x) {return (slope * (x - x1) + y1); };

			for(sint i = x1; i < x2; ++i)
			{
				if(line_width == 1)
				{
					draw(i, y(i), color);
				}
				else
				{
					size_t count = line_width;
					while(count > 0)
					{
						draw(i, y(i)+ceil(count/2),color);
						draw(i, y(i)-ceil(count--/2),color);
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
					drawLine(x, y(x) + centerY, x, -y(x) + centerY, color);
				}
			}

		}
		void Engine::drawCircle(const Point2D<sint>& center, const sint radius, 
				const Pixel& color, const bool fill) 
		{ drawCircle(center.x, center.y, radius, color, fill); } 

	};
};
