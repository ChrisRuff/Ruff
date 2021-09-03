#include "ruff/ui/image.hpp"

namespace ruff
{
namespace ui
{
void Image::set(const uint16_t x, const uint16_t y, const Pixel& color)
{
	pixels[y*w + x] = color;
}
Pixel Image::get(const uint16_t x, const uint16_t y) const
{
	return pixels[y*w + x];
}
void Image::line(const Point2D<uint16_t>& p1, const Point2D<uint16_t>& p2, const Pixel& color)
{
	bool steep = false;
	Point2D<uint16_t> start(p1);
	Point2D<uint16_t> end(p2);
	if(std::abs(start.x - end.x) < std::abs(start.y - end.y))
	{
		std::swap(start.x, start.y);
		std::swap(end.x, end.y);
	}
	if(start.x > end.x)
	{
		std::swap(start.x, end.x);
		std::swap(start.y, end.y);
	}
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int d_error = std::abs(dy) * 2;
	int error = 0;
	int y = 0;

	const int y_inc = (end.y > start.y ? 1 : -1);
	for(uint16_t x = start.x; x <= end.x; ++x)
	{
		if(steep)
		{
			set(y, x, color);
		}
		else
		{
			set(x, y, color);
		}
		error += d_error;
		if(error > dx)
		{
			y += y_inc;
			error -= dx * 2;
		}
	}
}
void Image::rectangle(const Point2D<uint16_t>& p1, const Point2D<uint16_t>& p2, const Pixel& color)
{
	uint16_t min_x = std::min(p1.x, p2.x);
	uint16_t min_y = std::min(p1.y, p2.y);
	uint16_t max_x = std::max(p1.x, p2.x);
	uint16_t max_y = std::max(p1.y, p2.y);
	for(uint16_t i = min_x; i < max_x; ++i)
	{
		for(uint16_t j = min_y; j < max_y; ++j)
		{
			set(i, j, color);
		}
	}
}
void Image::resize(uint16_t new_w, uint16_t new_h, RESAMP_METHOD method)
{
	(void)new_w;
	(void)new_h;
	(void)method;

}
void Image::overlay(const Image& overlay, uint16_t x, uint16_t y)
{
	for(uint16_t i = x; i < overlay.width()+x; ++i)
	{
		for(uint16_t j = y; j < overlay.height()+y; ++j)
		{
			set(i, j, overlay.get(i-x, j-y));
		}
	}
}
bool Image::write(const std::filesystem::path& out_path) const
{
	std::ofstream out_file(out_path, std::ios::binary);
	if(!out_file.is_open())
	{
		return false;
	}
	if(!out_path.extension().compare("ppm"))
	{
		out_file << "P6\n" << w << " " << h << "\n255\n";
		for(size_t i = 0; i < w * h; ++i)
		{
			const Pixel& p = pixels[i];
			out_file << p.r << p.g << p.b;
		}
		out_file.close();
	}
	else
	{
		std::cerr << "File type: " << 
			out_path.extension() << " is not supported" 
			<< std::endl;
	}
	return true;
}

Image Image::read(const std::filesystem::path& in_path)
{
	(void)in_path;
	return Image(5,5);
}
};
};
