#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>// ifstream and ofstream
#include <cstring>// memset

// Core
#include <ruff/core/logger.hpp>
#include <ruff/imgproc//pixel.hpp>
#include <ruff/core/structs/point.hpp>
#include <ruff/core/structs/tensor.hpp>


namespace ruff::imgproc
{
	class Image
	{
	private:
        core::Tensor<Pixel> m_pixels;

	public:
		Image(const uint16_t width,
		      const uint16_t height,
		      const std::vector<Pixel>& pixels)
		  : m_pixels(height, width)
		{
			for(size_t i = 0; i < Height(); ++i)
			{
                for(size_t j = 0; j < Width(); ++j)
                {
                    m_pixels(i, j) = pixels[j * Height() + i];
                }
			}
		}

		Image(const uint16_t width, const uint16_t height) : Image(width, height, WHITE) { }
		Image(const uint16_t width, const uint16_t height, Pixel bg)
		  : m_pixels(height, width) { m_pixels.Fill(bg); }
		Image() = default;

		[[nodiscard]] constexpr inline uint16_t Width() const { return m_pixels.cols(); };
		[[nodiscard]] constexpr inline uint16_t Height() const { return m_pixels.rows(); };

        // Querying
        std::vector<Pixel> Column(const uint16_t x, uint16_t height = 0) const;

        // Pixel editing
        void Set(const uint16_t x, const uint16_t y, const Pixel& color);
        Pixel Get(const uint16_t x, const uint16_t y) const;
        void Clear(const Pixel& p);
	public:
		std::vector<unsigned char> Data() const;
		const std::vector<Pixel>& GetPixels() const { return m_pixels.GetData(); }
	};
};// namespace ruff
