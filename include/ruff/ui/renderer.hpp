// Packages
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "ruff/ui/image.hpp"
#include "ruff/ui/destructors.hpp"
namespace ruff
{
namespace ui
{
	class Engine;
	struct MouseState
	{
		std::array<bool, 2> mouse_pressed{ false, false };
		std::array<bool, 2> mouse_held{ false, false };
		std::array<bool, 2> mouse_released{ false, false };
		int mouse_x{ 0 }, mouse_y{ 0 };
	};

#ifdef USE_SDL2
	class SDL2_Renderer
	{
	private:
		// SDL Variables used for rendering
		std::unique_ptr<SDL_Window, SDLDestroyer> window{ nullptr };
		std::unique_ptr<SDL_Renderer, SDLDestroyer> renderer{ nullptr };
		std::unique_ptr<SDL_Texture, SDLDestroyer> texture{ nullptr };

		Point2D<uint16_t> current_size;

		uint64_t last;

	public:
		SDL2_Renderer(const Point2D<uint16_t> size, const std::string& title) : current_size(size)
		{
			this->last = SDL_GetPerformanceCounter();
			if(SDL_Init(SDL_INIT_VIDEO) != 0)
			{
				ruff::logError(SDL_GetError());
			}
			else [[likely]]
			{
				window = std::unique_ptr<SDL_Window, SDLDestroyer>(
						SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

				renderer = std::unique_ptr<SDL_Renderer, SDLDestroyer>(
						SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

				texture = std::unique_ptr<SDL_Texture, SDLDestroyer>(
						SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ABGR8888, 
							SDL_TEXTUREACCESS_STREAMING, size.x, size.y));

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			}
		}

		~SDL2_Renderer()
		{
			SDL_Quit();
		}

		SDL_Renderer* getRenderer()
		{
			return renderer.get();
		}

		void resize(const Point2D<uint16_t> size)
		{
			texture.reset(SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ABGR8888, 
						SDL_TEXTUREACCESS_STREAMING, size.x, size.y));
			current_size = size;
		}

		void render(Image* image)
		{
			auto pixels = image->data();
			if(image->width() == current_size.x && image->height() == current_size.y)
			{
				SDL_RenderClear(renderer.get());
				SDL_UpdateTexture(texture.get(), nullptr, pixels.data(), image->width()*4);
				SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
				SDL_RenderPresent(renderer.get());
			}
		}
		void setTitle(const std::string& title)
		{
			SDL_SetWindowTitle(window.get(), title.c_str());
		}
		double getDeltaTime()
		{ 
			auto now = SDL_GetPerformanceCounter();
			double delta = static_cast<double>(now - this->last) * 100 / static_cast<double>(SDL_GetPerformanceFrequency());
			this->last = now;
			return delta;
		}
	};

};
};
#endif
