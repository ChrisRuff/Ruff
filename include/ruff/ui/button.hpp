#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ruff/ui/destructors.hpp"
#include "ruff/ui/text.hpp"
#include "ruff/ui/pixel.hpp"

namespace ruff
{
namespace ui
{

	using sint = uint16_t;
	/* --------------------------------------------------------------------------*/
	/**
		 * @Synopsis Represents a button for storing inputs
		 */
	/* ----------------------------------------------------------------------------*/
	class Button
	{
	private:
		sint x{};
		sint y{};
		int width{};
		int height{};
		int pixelRatio{};

		std::string label{ "" };
		std::unique_ptr<TTF_Font, SDLDestroyer> font{ nullptr };

		bool pressed{};
		bool changed{};

		Pixel color{};

	public:
		Button() = default;

		Button(const sint x, 
				const sint y, 
				const int width, 
				const int height, 
				const Pixel color, 
				const int pixelRatio, 
				const std::string& fontPath = "", 
				const std::string& label = "", 
				const int fontSize = 12)
		  : x(x), y(y), width(width),
		    height(height), pixelRatio(pixelRatio), label(label),
		    font(makeFont(fontPath, fontSize)), color(color)
		{
		}
		Button(const Button&) = delete;

		sint getX() { return x; };
		sint getY() { return y; };
		int getWidth() { return width; };
		int getHeight() { return height; };
		int getPixelRatio() { return pixelRatio; };
		Pixel& getColor() { return color; };
		std::string& getLabel() { return label; };
		TTF_Font* getFont() { return font.get(); };

		bool isPressed() { return pressed; };
		void press()
		{
			pressed = !pressed;

			// TODO: CHANGE COLOR
			if(pressed)
			{
				color = Pixel(color.r + 100, color.g + 100, color.b + 100, color.a);
			}
			else
			{
				color = Pixel(color.r - 100, color.g - 100, color.b - 100, color.a);
			}
			changed = true;
		};
		bool isChanged() 
		{ 
			bool c = changed;
			changed = false;
			return c; 
		};
	};
};// namespace ui
};// namespace ruff
