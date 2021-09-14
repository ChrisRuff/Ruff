// Packages
#include <unordered_map>

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
	enum class Key
	{
		NONE,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		K0,
		K1,
		K2,
		K3,
		K4,
		K5,
		K6,
		K7,
		K8,
		K9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE,
		TAB,
		SHIFT,
		CTRL,
		INS,
		DEL,
		HOME,
		END,
		PGUP,
		PGDN,
		BACK,
		ESCAPE,
		RETURN,
		ENTER,
		PAUSE,
		SCROLL,
		NP0,
		NP1,
		NP2,
		NP3,
		NP4,
		NP5,
		NP6,
		NP7,
		NP8,
		NP9,
		NP_MUL,
		NP_DIV,
		NP_ADD,
		NP_SUB,
		NP_DECIMAL,
		PERIOD,
		EQUALS,
		COMMA,
		MINUS,
		OEM_1,
		OEM_2,
		OEM_3,
		OEM_4,
		OEM_5,
		OEM_6,
		OEM_7,
		OEM_8,
		CAPS_LOCK,
		ENUM_END
	};
	static std::unordered_map<size_t, Key> key_map{};

#ifdef USE_SDL2
	class SDL2_Renderer
	{
	private:
		// SDL Variables used for rendering
		std::unique_ptr<SDL_Window, SDLDestroyer> window{ nullptr };
		std::unique_ptr<SDL_Renderer, SDLDestroyer> renderer{ nullptr };
		std::unique_ptr<SDL_Texture, SDLDestroyer> texture{ nullptr };

		Point2D<uint16_t> current_size;

		uint64_t last{0};

	public:
		SDL2_Renderer(const Point2D<uint16_t> size,
		              const std::string& title)
		  : current_size(size)
		{
			this->last = SDL_GetPerformanceCounter();
			if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
			{
				ruff::logError(SDL_GetError());
			}
			else [[likely]]
			{
				window = std::unique_ptr<SDL_Window, SDLDestroyer>(
				  SDL_CreateWindow(title.c_str(),
				                   SDL_WINDOWPOS_CENTERED,
				                   SDL_WINDOWPOS_CENTERED,
				                   size.x,
				                   size.y,
				                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

				renderer = std::unique_ptr<SDL_Renderer, SDLDestroyer>(
				  SDL_CreateRenderer(
				    window.get(), -1, SDL_RENDERER_ACCELERATED));

				texture = std::unique_ptr<SDL_Texture, SDLDestroyer>(
				  SDL_CreateTexture(renderer.get(),
				                    SDL_PIXELFORMAT_ABGR8888,
				                    SDL_TEXTUREACCESS_STREAMING,
				                    size.x,
				                    size.y));

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				                    SDL_GL_CONTEXT_PROFILE_CORE);
			}
			key_map[SDLK_a] = Key::A;
			key_map[SDLK_b] = Key::B;
			key_map[SDLK_c] = Key::C;
			key_map[SDLK_d] = Key::D;
			key_map[SDLK_e] = Key::E;
			key_map[SDLK_f] = Key::F;
			key_map[SDLK_g] = Key::G;
			key_map[SDLK_h] = Key::H;
			key_map[SDLK_i] = Key::I;
			key_map[SDLK_j] = Key::J;
			key_map[SDLK_k] = Key::K;
			key_map[SDLK_l] = Key::L;
			key_map[SDLK_m] = Key::M;
			key_map[SDLK_n] = Key::N;
			key_map[SDLK_o] = Key::O;
			key_map[SDLK_p] = Key::P;
			key_map[SDLK_q] = Key::Q;
			key_map[SDLK_r] = Key::R;
			key_map[SDLK_s] = Key::S;
			key_map[SDLK_t] = Key::T;
			key_map[SDLK_u] = Key::U;
			key_map[SDLK_v] = Key::V;
			key_map[SDLK_w] = Key::W;
			key_map[SDLK_x] = Key::X;
			key_map[SDLK_y] = Key::Y;
			key_map[SDLK_z] = Key::Z;

			key_map[SDLK_F1] = Key::F1;
			key_map[SDLK_F2] = Key::F2;
			key_map[SDLK_F3] = Key::F3;
			key_map[SDLK_F4] = Key::F4;
			key_map[SDLK_F5] = Key::F5;
			key_map[SDLK_F6] = Key::F6;
			key_map[SDLK_F7] = Key::F7;
			key_map[SDLK_F8] = Key::F8;
			key_map[SDLK_F9] = Key::F9;
			key_map[SDLK_F10] = Key::F10;
			key_map[SDLK_F11] = Key::F11;
			key_map[SDLK_F12] = Key::F12;

			key_map[SDLK_DOWN] = Key::DOWN;
			key_map[SDLK_UP] = Key::UP;
			key_map[SDLK_LEFT] = Key::LEFT;
			key_map[SDLK_RIGHT] = Key::RIGHT;

			key_map[SDLK_BACKSPACE] = Key::BACK;
			key_map[SDLK_ESCAPE] = Key::ESCAPE;
			key_map[SDLK_TAB] = Key::TAB;
			key_map[SDLK_RETURN] = Key::ENTER;
		}
		~SDL2_Renderer() { SDL_Quit(); }

		SDL_Renderer* getRenderer() { return renderer.get(); }

		void resize(const Point2D<uint16_t>& size)
		{
			texture.reset(SDL_CreateTexture(renderer.get(),
			                                SDL_PIXELFORMAT_ABGR8888,
			                                SDL_TEXTUREACCESS_STREAMING,
			                                size.x,
			                                size.y));
			current_size = size;
		}

		void render(Image* image)
		{
			auto pixels = image->data();
			if(image->width() == current_size.x
			   && image->height() == current_size.y)
			{
				SDL_RenderClear(renderer.get());
				SDL_UpdateTexture(
				  texture.get(), nullptr, pixels.data(), image->width() * 4);
				SDL_RenderCopy(
				  renderer.get(), texture.get(), nullptr, nullptr);
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
			double delta =
			  static_cast<double>(now - this->last) * 100
			  / static_cast<double>(SDL_GetPerformanceFrequency());
			this->last = now;
			return delta;
		}
	};
};
};
#endif