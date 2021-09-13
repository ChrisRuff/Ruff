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

	/* --------------------------------------------------------------------------*/
	/**
		 * @Synopsis Represents a button for storing inputs
		 */
	/* ----------------------------------------------------------------------------*/
struct Button
{
	Point2D<uint16_t> pos;
	Point2D<uint16_t> size;

	std::string label{ "" };
	Pixel color{};
	Pixel active_color{};

	bool pressed{false};

	Button(const Point2D<uint16_t> pos, 
			const Point2D<uint16_t> size) :
		pos(pos), size(size)
	{
	}
	Button(const Button&) = delete;

	Point2D<uint16_t> xy() const { return pos; };
	Point2D<uint16_t> dims() const { return size; };
	Pixel getColor() const { return pressed ? this->active_color : this->color; };
	std::string& getLabel() { return label; };

	void setColor(const Pixel& pixel)
	{
		this->color = pixel;
		this->active_color = Pixel(
				pixel.r + 100,
				pixel.g + 100,
				pixel.b + 100,
				pixel.a);
	}
	void setActiveColor(const Pixel& pixel)
	{
		this->active_color = pixel;
	}

	bool isPressed() { return pressed; };
	void press() { pressed = !pressed; };
};
};// namespace ui
};// namespace ruff
