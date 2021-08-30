#pragma once
#include <memory>

#include "ruff/ui/destructors.hpp"
#include "ruff/core/logger.hpp"

namespace ruff
{
namespace ui
{
	std::unique_ptr<TTF_Font, SDLDestroyer> makeFont(std::string path, int fontsize);
}
}// namespace ruff
