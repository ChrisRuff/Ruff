#include "ruff/ui/text.hpp"

namespace ruff
{
namespace ui
{
	std::unique_ptr<TTF_Font, SDLDestroyer> makeFont(std::string path, int fontsize)
	{
		if(path.size() == 0)
			return nullptr;

		auto font = std::unique_ptr<TTF_Font, SDLDestroyer>(TTF_OpenFont(path.c_str(), fontsize));
		if(!font.get())
		{
			logWarning("Could not initialize font " + path);
		}
		return font;
	}
}// namespace ui
}// namespace ruff
