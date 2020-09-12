#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ruff
{
	namespace ui
	{
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Struct to destroy SDL object pointers, this is mainly useful for 
		 * storing SDL object pointers as a unique_ptr.
		 * Syntax: std::unique_ptr<SDL_XXXXXX, SDLDestroyer> ptr;
		 */
		/* ----------------------------------------------------------------------------*/
		struct SDLDestroyer 
		{
			void operator()(SDL_Surface*  ptr) { if (ptr) SDL_FreeSurface(ptr); }
			void operator()(SDL_Texture*  ptr) { if (ptr) SDL_DestroyTexture(ptr); }
			void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
			void operator()(SDL_Window*   ptr) { if (ptr) SDL_DestroyWindow(ptr); }
			void operator()(SDL_RWops*    ptr) { if (ptr) SDL_RWclose(ptr); }
			void operator()(TTF_Font*     ptr) { (void)ptr; /* if (ptr) TTF_CloseFont(ptr); */ }
		};
	}
}
