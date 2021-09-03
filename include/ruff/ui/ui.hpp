#pragma once

//STL packages
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <string_view>

#include <thread>
#include <chrono>

#include <iostream>

// Packages
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>

// Source
#include "ruff/ui/destructors.hpp"
#include "ruff/ui/text.hpp"
#include "ruff/ui/pixel.hpp"
#include "ruff/ui/button.hpp"
#include "ruff/geometry/point.hpp"
#include "ruff/core/logger.hpp"

namespace ruff
{
namespace ui
{
	struct MouseState
	{
		std::array<bool, 2> mouse_pressed{ false, false };
		std::array<bool, 2> mouse_held{ false, false };
		std::array<bool, 2> mouse_released{ false, false };
		int mouse_x{ 0 }, mouse_y{ 0 };
	};

	/* --------------------------------------------------------------------------*/
	/**
		 * @Synopsis UI engine to render pixels
		 */
	/* ----------------------------------------------------------------------------*/
	class Engine
	{
	protected:
		uint16_t width{};
		uint16_t height{};
		std::string title{};
		uint16_t screenWidth{};
		uint16_t screenHeight{};
		uint16_t pixelRatio{};

		// Hardware interface
		MouseState mouse{};

		// Access the keyboard keys with keyboard_buttons with
		// keys[SDLK_~] -> to detect when r is pressed I would do
		// if(keys[SDLK_r])
		std::array<bool, 322> keys{};

		// All pixels in the image
		std::vector<unsigned char> pixels{};

		std::vector<std::unique_ptr<Button>> buttons{};

		// SDL Variables used for rendering
		std::unique_ptr<SDL_Window, SDLDestroyer> window{ nullptr };
		std::unique_ptr<SDL_Renderer, SDLDestroyer> renderer{ nullptr };
		std::unique_ptr<SDL_Texture, SDLDestroyer> texture{ nullptr };
		std::vector<std::unique_ptr<SDL_Texture, SDLDestroyer>> sprites{};

		SDL_GLContext gl_context{};

	public:
		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Creates an Engine with window height, width and title.
			 *
			 * @Param width Width of the window created
			 * @Param height Height of the window created
			 * @Param title Title of created window
			 */
		/* ----------------------------------------------------------------------------*/
		Engine(const uint16_t width, const uint16_t height, std::string title = "Window", int pixelRatio = 1);

		virtual ~Engine() = default;

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis Copy constructor doesn't exist because of unique data members
			 *
			 * @Param other 
			 */
		/* ----------------------------------------------------------------------------*/
		Engine(const Engine& other) = delete;

		Engine& operator=(const Engine& other) = delete;

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis Loads a sprite into the engine to be referenced
			 *
			 * @Param filepath Location of the sprite 
			 *
			 * @Returns index of the sprite in the sprites vector
			 */
		/* ----------------------------------------------------------------------------*/
		int loadSprite(const std::string& filepath);

		std::unordered_map<std::string, int> getSpriteInfo(int index);

		void displaySprite(const uint16_t x, const uint16_t y, const int idx, const int scale, const double angle = 0, const int rX = -1, const int rY = -1);

		std::vector<Point2D<uint16_t>> static getLine(const uint16_t x1, const uint16_t y1, 
				const uint16_t x2, const uint16_t y2, const int line_width);

		std::vector<Point2D<uint16_t>> static getLine(const Point2D<uint16_t> p1, 
				const Point2D<uint16_t> p2, const int line_width);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Draws a line from (x1,y1) to (x2,y2)
			 *
			 * @Param x1 Start x coordinate of the line
			 * @Param y1 Start y coordinate of the line
			 * @Param x2 End x coordinate of the line
			 * @Param y2 End y coordinate of the line
			 * @Param color Color of the line
			 * @Param line_width How wide the line will be drawn
			 */
		/* ----------------------------------------------------------------------------*/
		void drawLine(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2, const Pixel& color = WHITE, const int line_width = 1);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Helper function to the above drawLine, This method takes two points
			 * as the start and end
			 *
			 * @Param p1 Start pixel
			 * @Param p2 End pixel
			 * @Param color Color of the line
			 */
		/* ----------------------------------------------------------------------------*/
		void drawLine(const Point2D<uint16_t>& p1, const Point2D<uint16_t>& p2, const Pixel& color, const int line_width);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Changed the color of a single pixel to the one given 
			 *
			 * @Param x X coordinate of the pixel
			 * @Param y Y coordinate of the pixel
			 * @Param color The color that will replace the pixel
			 */
		/* ----------------------------------------------------------------------------*/
		void draw(const uint16_t x, const uint16_t y, const Pixel& color);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Helper function of the above that takes a pixel
			 *
			 * @Param p Pixel location
			 * @Param color The color that will replace the pixel
			 */
		/* ----------------------------------------------------------------------------*/
		void draw(const Point2D<uint16_t>& p, const Pixel& color);

		void drawSquare(const uint16_t leftX, const uint16_t leftY, const uint16_t rightX, const uint16_t rightY, const Pixel& color, const bool fill = false);

		void drawSquare(const Point2D<uint16_t>& left, const Point2D<uint16_t>& right, const Pixel& color, const bool fill = false);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Draws a circle centered on the given X and Y coordinates
			 *
			 * @Param centerX Position of the center of the circle
			 * @Param centerY Position of the center of the circle
			 * @Param radius Radius of the circle 
			 * @Param color Color that the circle will be drawn with
			 * @Param fill Whether or not to fill the circle or just 
			 * draw the circumference(default to no-fill)
			 */
		/* ----------------------------------------------------------------------------*/
		void drawCircle(const uint16_t centerX, const uint16_t centerY, const uint16_t radius, const Pixel& color, const bool fill = false);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Helper function that takes points objects
			 *
			 * @Param center Point that the circle center is
			 * @Param radius Radius of the circle
			 * @Param color Color that the circle will be drawn with
			 * @Param fill Whether or not to fill the circle or just 
			 * draw the circumference(default to no-fill)
			 */
		/* ----------------------------------------------------------------------------*/
		void drawCircle(const Point2D<uint16_t>& center, const uint16_t radius, const Pixel& color, bool fill = false);

		void drawButton(Button* button);

		int addButton(uint16_t x, uint16_t y, int width, int height, Pixel color, int pixelRatio, std::string fontPath = "", std::string label = "", int fontSize = 12);

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Returns the width with the pixel ratio in mind
			 *
			 * @Returns   
			 */
		/* ----------------------------------------------------------------------------*/
		[[nodiscard]] uint16_t getWidth() const
		{
			return width / pixelRatio;
		}

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Returns the width with the pixel ratio in mind
			 *
			 * @Returns   
			 */
		/* ----------------------------------------------------------------------------*/
		[[nodiscard]] uint16_t getHeight() const
		{
			return height / pixelRatio;
		}

		Pixel getPixel(uint16_t x, uint16_t y) const;
		Pixel getPixel(Point2D<uint16_t> p) const;
		std::vector<Pixel> getRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		std::vector<Pixel> getRegion(Point2D<uint16_t> p1, Point2D<uint16_t> p2);

		[[nodiscard]] bool onButton(const MouseState& mouse) const;

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Replaces every pixel on the screen with the given color
			 *
			 * @Param color Color that will fill every pixel(default to opaque black)
			 */
		/* ----------------------------------------------------------------------------*/
		void clearScreen(Pixel color = Pixel(0, 0, 0, 1));

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Initializes the SDL and OpenGL environment 
			 * Begins the game engine
			 */
		/* ----------------------------------------------------------------------------*/
		void launch();

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Must be overridden by sub-classes to change the pixels on the 
			 * screen. This method is ran at the beginning of the program, and is ONLY RAN 
			 * ONCE.
			 */
		/* ----------------------------------------------------------------------------*/
		virtual void onCreate() = 0;

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Must be overridden by sub-classes to change the pixels on the 
			 * screen. This method is ran every frame.
			 */
		/* ----------------------------------------------------------------------------*/
		virtual void onUpdate(double deltaTime) = 0;

		/* --------------------------------------------------------------------------*/
		/**
			 * @Synopsis  Finishes the software, stopping all SDL processes. Override to 
			 * return true when you want the software to finish, if this isn't overridden
			 * then the software will continue until the program is finished forcefully
			 * by the user.
			 *
			 * @Returns bool
			 */
		/* ----------------------------------------------------------------------------*/
		virtual bool close() { return false; }
	};

	Pixel getRandColor();
};// namespace ui
};// namespace ruff
