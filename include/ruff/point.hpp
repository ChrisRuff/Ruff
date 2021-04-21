#pragma once
#include <ostream>
#include <cmath>// pow

#include <compare>    // partial_ordering
#include <concepts>   // requires
#include <type_traits>// is_arithmetic_v

namespace ruff
{
template<typename T>
requires std::is_arithmetic_v<T> class Point2D
{
public:
	T x{};
	T y{};


public:
	[[nodiscard]] Point2D(const T x, const T y) noexcept : x(x), y(y)
	{
	}

	[[nodiscard]] Point2D() = default;

	inline constexpr auto operator<=>(const Point2D<T>& other) const noexcept
	{
		// Compare based on distance from origin
		double length1 = std::pow(x, 2) + std::pow(y, 2);
		double length2 = std::pow(other.x, 2) + std::pow(other.y, 2);
		return (length1 <=> length2);
	}

	inline friend std::ostream& operator<<(std::ostream& os, const Point2D<T>& point)
	{
		return (os << "[" << point.x << ", " << point.y << "]" << std::endl);
	}
	inline friend bool operator==(const Point2D<T>& left, const Point2D<T>& right)
	{
		return (left.x == right.x && left.y == right.y);
	}
	inline friend bool operator!=(const Point2D<T>& left, const Point2D<T>& right)
	{
		return !(left.x == right.x && left.y == right.y);
	}
};
};// namespace ruff
