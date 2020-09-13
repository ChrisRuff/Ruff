#pragma once
#include <memory>

#include "ui/destructors.hpp"
#include "logger.hpp"

namespace ruff
{
namespace ui
{
	std::unique_ptr<TTF_Font, SDLDestroyer> makeFont(std::string path, int fontsize);
}
}// namespace ruff
