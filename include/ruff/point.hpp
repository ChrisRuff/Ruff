#pragma once
#include <ostream>
#include <type_traits>

//TODO c++2a:
//#include <compare>
namespace ruff
{
//TODO c++2a:
template<typename T>//requires std::is_arithmetic<T>
class Point2D
{
 public:
	T x{};
	T y{};


 public:
	Point2D(const T x, const T y) noexcept : x(x), y(y)
	{
	}
	Point2D() = default;

	//TODO c++2a:
	/*
		inline constexpr std::partial_ordering operator<=>(const Point2D<T>& other) const
		{
			double length1 = std::pow(x,2) + std::pow(y,2);
			double length2 = std::pow(other.x,2) + std::pow(other.y,2);
			return (length1 <=> length2);
		}
		*/
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
