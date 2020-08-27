#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ui/destructors.hpp"
#include "ui/text.hpp"
#include "ui/pixel.hpp"

namespace ruff
{
	namespace ui
	{

		using sint = short int;
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

			std::string label{""};
			std::unique_ptr<TTF_Font, SDLDestroyer> font{nullptr};

			bool pressed{};
			Pixel color{};

		public:
			Button() = default;

			Button(sint x, sint y, int width, int height, 
					Pixel color, int pixelRatio, std::string fontPath = "",
					std::string label = "", int fontSize = 12) 
				: x(x), y(y), width(width), 
				height(height), pixelRatio(pixelRatio), label(label), 
				font(makeFont(fontPath, fontSize)), color(color)
			{

			}
			Button (const Button&) = delete;	

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
					color = Pixel(color.r+100, color.g+100, color.b+100, color.a);
				}
				else
				{
					color = Pixel(color.r-100, color.g-100, color.b-100, color.a);
				}
			};

		};
	};
};
