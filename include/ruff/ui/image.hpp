#pragma once
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>// ifstream and ofstream
#include <cstring>// memset

#include "png.h"

// Core
#include "ruff/core/logger.hpp"

// UI
#include "ruff/ui/pixel.hpp"

// Geometry
#include "ruff/geometry/point.hpp"
#include "ruff/geometry/mesh.hpp"

namespace ruff
{
namespace ui
{
	enum class RESAMP_METHOD
	{
		NEAREST_NEIGHBOR
	};
	enum class ROTATION_METHOD
	{
		PARTIAL_SHEAR,
		SHEAR
	};
	class Image
	{
	private:
		uint16_t w{};
		uint16_t h{};
		std::vector<Pixel> pixels{};

	public:
		Image(const uint16_t width,
		      const uint16_t height,
		      const std::vector<Pixel>& pixels)
		  : w(width), h(height), pixels(width * height)
		{
			for(size_t i = 0; i < pixels.size(); ++i)
			{
				this->pixels[i] = pixels[i];
			}
		}

		Image(const uint16_t width, const uint16_t height)
		  : w(width), h(height),
		    pixels(width * height, Pixel(255, 255, 255))
		{
		}

		Image(const uint16_t width, const uint16_t height, Pixel bg)
		  : w(width), h(height), pixels(width * height, bg)
		{
		}

		Image() = default;

		[[nodiscard]] constexpr uint16_t width() const { return w; };
		[[nodiscard]] constexpr uint16_t height() const { return h; };

		// Querying
		std::vector<Pixel> column(const uint16_t x,
		                          uint16_t height = 0) const;
		std::vector<unsigned char> data() const;

		// Image manipulation
		Image rotate(const double theta,
		             const ROTATION_METHOD rotation_method =
		               ROTATION_METHOD::SHEAR) const;
		void resize(
		  uint16_t new_w,
		  uint16_t new_h,
		  const RESAMP_METHOD method = RESAMP_METHOD::NEAREST_NEIGHBOR);
		void
		  overlay(const Image& overlay, uint16_t x = 0, uint16_t y = 0);

		// Pixel editing
		void set(const uint16_t x, const uint16_t y, const Pixel& color);
		Pixel get(const uint16_t x, const uint16_t y) const;
		void line(const Point2D<uint16_t>& p1,
		          const Point2D<uint16_t>& p2,
		          const Pixel& color);
		void rectangle(const Point2D<uint16_t>& p1,
		               const Point2D<uint16_t>& p2,
		               const Pixel& color);

		// IO
		bool write(const std::filesystem::path& out_path) const;
		[[nodiscard]] Image static read(
		  const std::filesystem::path& in_path);

		void clear(const Pixel& p);
	};
};// namespace ui
};// namespace ruff
