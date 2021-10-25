#pragma once
#include <cstdlib>
#include <compare>// partial_ordering
#include <string>
#include "ruff/core/logger.hpp"

namespace ruff
{
namespace ui
{
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis A pixel represents a
	 * color of RGB and alpha Alpha of 0
	 * is transparent, and alpha of 1 is
	 * opaque
	 */
	/* ----------------------------------------------------------------------------*/
	class Pixel
	{
	public:
		uint8_t r{ 0 }, g{ 0 }, b{ 0 }, a{ 1 };

	public:
		[[nodiscard]] constexpr Pixel() = default;

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Constructs a pixel of
		 * given grayscale color and alpha
		 * of 1
		 *
		 * @Param g Grayscale value
		 */
		/* ----------------------------------------------------------------------------*/
		[[nodiscard]] constexpr Pixel(uint8_t g) noexcept
		  : r(g), g(g), b(g)
		{
		}
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Constructs a pixel of
		 * given RGB and alpha of 1
		 *
		 * @Param r
		 * @Param g
		 * @Param b
		 */
		/* ----------------------------------------------------------------------------*/
		[[nodiscard]] constexpr Pixel(uint8_t r,
		                              uint8_t g,
		                              uint8_t b) noexcept
		  : r(r), g(g), b(b)
		{
		}
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Constructs a pixel of
		 * given RGBA
		 *
		 * @Param r
		 * @Param g
		 * @Param b
		 * @Param a
		 */
		/* ----------------------------------------------------------------------------*/
		[[nodiscard]] constexpr Pixel(uint8_t r,
		                              uint8_t g,
		                              uint8_t b,
		                              uint8_t a) noexcept
		  : r(r), g(g), b(b), a(a)
		{
		}

		static Pixel combine(const Pixel& old_p, const Pixel& new_p)
		{
			Pixel p((old_p.r * (1 - new_p.a)) + (new_p.r * new_p.a),
			        (old_p.g * (1 - new_p.a)) + (new_p.g * new_p.a),
			        (old_p.b * (1 - new_p.a)) + (new_p.b * new_p.a),
			        1);
			return p;
		}

		~Pixel() = default;

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Adds indexing a pixels
		 * individual color values by []
		 *
		 * @Param index Index of the pixel
		 * 0 - red
		 * 1 - green
		 * 2 - blue
		 * 3 - alpha
		 * Any index higher than 3 is
		 * reduced to a number less than
		 * 4(index % 4) such as 4 -> 0 5 ->
		 * 1 6 -> 2 ...
		 * @Returns
		 */
		/* ----------------------------------------------------------------------------*/
		uint8_t operator[](size_t index)
		{
			index = index % 4;
			if(index == 0)
				return r;
			else if(index == 1)
				return g;
			else if(index == 2)
				return b;
			else
				return a;
		}

		inline bool operator==(const Pixel& other) const noexcept
		{
			return r == other.r && g == other.g && b == other.b
			       && a == other.a;
		}

		operator std::string() const
		{
			return "[" + std::to_string(static_cast<unsigned int>(r)) + ", "
			       + std::to_string(static_cast<unsigned int>(g)) + ", "
			       + std::to_string(static_cast<unsigned int>(b)) + ", "
			       + std::to_string(static_cast<unsigned int>(a)) + "]";
		};
	};

	// List of some colours
	static constexpr Pixel GREY(192, 192, 192),
	  DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
	  RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
	  YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0),
	  VERY_DARK_YELLOW(64, 64, 0), GREEN(0, 255, 0),
	  DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
	  CYAN(0, 255, 255), DARK_CYAN(0, 128, 128),
	  VERY_DARK_CYAN(0, 64, 64), BLUE(0, 0, 255), DARK_BLUE(0, 0, 128),
	  VERY_DARK_BLUE(0, 0, 64), MAGENTA(255, 0, 255),
	  DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
	  WHITE(255, 255, 255), BLACK(0, 0, 0), BLANK(0, 0, 0, 0);

};// namespace ui
};// namespace ruff
