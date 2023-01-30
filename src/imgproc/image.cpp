#include <ruff/imgproc/image.hpp>

namespace ruff::imgproc
{
	void Image::Set(const uint16_t x, const uint16_t y, const Pixel& color)
	{
		if(x >= Width() || y >= Height()) { return; }
		m_pixels(y, x) = color;
	}
	Pixel Image::Get(const uint16_t x, const uint16_t y) const
	{
		if(x >= Width() || y >= Height()) { return BLANK; }
		return m_pixels(y, x);
	}

	std::vector<Pixel> Image::Column(const uint16_t x,
	                                 uint16_t height) const
	{
		if(height == 0) { height = Height(); }

		std::vector<Pixel> column(height);
#pragma omp parallel for
		for(size_t y = 0; y < height; ++y)
		{
			const size_t pix_y = (y * Height()) / height;
			column[y] = Get(x, pix_y);
		}
		auto end = column.end();
		column.insert(end, column.begin(), column.end());

		return column;
	}
	std::vector<unsigned char> Image::Data() const
	{
		std::vector<unsigned char> data(m_pixels.Size() * 4);

#pragma omp parallel for
		for(size_t i = 0; i < data.size(); i += 4)
		{
			const size_t idx = i / 4;
			data[i] =     m_pixels[idx].r;
			data[i + 1] = m_pixels[idx].g;
			data[i + 2] = m_pixels[idx].b;
			data[i + 3] = m_pixels[idx].a;
		}
		return data;

	}
	std::vector<unsigned char> Image::BGRData() const
	{
			std::vector<unsigned char> data(m_pixels.Size() * 4);

#pragma omp parallel for
			for(size_t i = 0; i < data.size(); i += 4)
			{
					const size_t idx = i / 4;
					data[i] =     m_pixels[idx].b;
					data[i + 1] = m_pixels[idx].g;
					data[i + 2] = m_pixels[idx].r;
					data[i + 3] = m_pixels[idx].a;
			}
			return data;

	}

	void Image::Clear(const Pixel& p)
	{
		std::fill(m_pixels.begin(), m_pixels.end(), p);
	}
};// namespace ruff
