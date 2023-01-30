#pragma once
#include <utility>
#include <functional>
#include <type_traits>

#include <ruff/core/models/point.hpp>

namespace ruff
{
namespace geometry
{
	class Line
	{
	public:
		std::pair<size_t, size_t> line;

	public:
		Line(size_t idx1, size_t idx2) noexcept : line({ idx1, idx2 }) {}
		size_t start() const { return line.first; };
		size_t end() const { return line.second; };
	};
};// namespace geometry
};// namespace ruff
