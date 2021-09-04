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
void Image::rotate(const double rotation, ROTATION_METHOD rotation_method)
{
	(void)rotation;
}
void Image::overlay(const Image& overlay, uint16_t x, uint16_t y)
{
	for(uint16_t i = x; i < overlay.width()+x && i < w; ++i)
	{
		for(uint16_t j = y; j < overlay.height()+y && j < h; ++j)
		{
			set(i, j, overlay.get(i-x, j-y));
		}
	}
}
bool Image::write(const std::filesystem::path& out_path) const
{
	if(out_path.extension() == ".png")
	{

	}
	else if(out_path.extension() == ".ppm")
	{
		std::ofstream out_file(out_path, std::ios::binary);
		if(!out_file.is_open())
		{
			return false;
		}
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
	if(in_path.extension() == ".ppm")
	{
		throw std::runtime_error("Filetype not supported");
	}
	else if(in_path.extension() == ".png")
	{
		if(!std::filesystem::exists(in_path))
		{
			throw std::runtime_error("No such file found: " + in_path.string());
		}
		// https://fossies.org/linux/libpng/example.c
		png_image png_image;
		memset(&png_image, 0, sizeof(png_image));
		png_image.version = PNG_IMAGE_VERSION;
		
		if(png_image_begin_read_from_file(&png_image, in_path.c_str()) != 0)
		{
			png_image.format = PNG_FORMAT_RGBA;

			png_bytep buffer = 
				static_cast<unsigned char*>(malloc(PNG_IMAGE_SIZE(png_image)));
			if(buffer != NULL &&
					png_image_finish_read(&png_image, NULL, buffer, 0, NULL) != 0)
			{
				// Convert png image to pixels
				std::vector<Pixel> pixels{};
				for(size_t i = 0; i < PNG_IMAGE_SIZE(png_image); i+=4)
				{
					pixels.emplace_back(
						buffer[i],
						buffer[i+1],
						buffer[i+2],
						buffer[i+3]
					);
				}
				Image image(png_image.width, png_image.height, pixels);
				return image;
			}
			else
			{
				if(buffer == NULL)
				{
					png_image_free(&png_image);
				}
				else
				{
					free(buffer);
				}
				throw std::runtime_error("Could not allocate image");
			}
		}
		else
		{
			throw std::runtime_error("Could not read PNG file");
		}
	}
	else
	{
		ruff::logError("Filetype not supported");
		throw std::runtime_error("Filetype not supported");
	}
}
};
};
