#include <uchar.h>
namespace ruff
{
	namespace ui
	{
		using uchar = unsigned char;
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis A pixel represents a color of RGB and alpha
		 * Alpha of 0 is transparent, and alpha of 1 is opaque
		 */
		/* ----------------------------------------------------------------------------*/
		class Pixel 
		{
		public:
			uchar r{0}, g{0}, b{0}, a{1};
		public:
			Pixel() = default;
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Constructs a pixel of given RGB and alpha of 1
			 *
			 * @Param r
			 * @Param g
			 * @Param b
			 */
			/* ----------------------------------------------------------------------------*/
			Pixel(uchar r, uchar g, uchar b) : r(r), g(g), b(b)
			{

			}
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Constructs a pixel of given RGBA
			 *
			 * @Param r
			 * @Param g
			 * @Param b
			 * @Param a
			 */
			/* ----------------------------------------------------------------------------*/
			Pixel(uchar r, uchar g, uchar b, uchar a) : r(r), g(g), b(b), a(a)
			{

			}

			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Adds indexing a pixels individual color values by []
			 *
			 * @Param index Index of the pixel
			 * 0 - red
			 * 1 - green
			 * 2 - blue
			 * 3 - alpha
			 * Any index higher than 3 is reduced to a number less than 4(index % 4) such as 
			 * 4 -> 0
			 * 5 -> 1
			 * 6 -> 2 ...
			 * @Returns   
			 */
			/* ----------------------------------------------------------------------------*/
			uchar operator[](size_t index) 
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
		};

		// List of some colours
		static const Pixel
			GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
			RED(255, 0, 0),      DARK_RED(128, 0, 0),      VERY_DARK_RED(64, 0, 0),
			YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
			GREEN(0, 255, 0),    DARK_GREEN(0, 128, 0),    VERY_DARK_GREEN(0, 64, 0),
			CYAN(0, 255, 255),   DARK_CYAN(0, 128, 128),   VERY_DARK_CYAN(0, 64, 64),
			BLUE(0, 0, 255),     DARK_BLUE(0, 0, 128),     VERY_DARK_BLUE(0, 0, 64),
			MAGENTA(255, 0, 255),DARK_MAGENTA(128, 0, 128),VERY_DARK_MAGENTA(64, 0, 64),
			WHITE(255, 255, 255),BLACK(0, 0, 0),           BLANK(0, 0, 0, 0);
	};
};
