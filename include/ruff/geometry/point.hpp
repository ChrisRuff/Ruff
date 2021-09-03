#pragma once
#include <ostream>
#include <cmath>// pow

#include <array>

#include <compare>    // partial_ordering
#include <concepts>   // requires
#include <type_traits>// is_arithmetic_v

namespace ruff
{
template<typename T, size_t size>
requires std::is_arithmetic_v<T> class Point
{
public:
	std::array<T, size> vals{};

public:
	[[nodiscard]] constexpr Point(std::array<T, size> vals = {}) noexcept : vals(vals)
	{
	}

	inline constexpr auto operator<=>(const Point<T, size>& other) const noexcept
	{
		// Compare based on distance from origin
		double length1 = 0;
		double length2 = 0;
		std::for_each(vals.begin(), vals.end(), [length1](const T& val){ length1 += std::pow(val, 2); });
		for(size_t i = 0; i < size; ++i)
		{
			length2 += std::pow(other[i], 2);
		}
		return (length1 <=> length2);
	}
	T& operator[](size_t idx) 
	{
		return vals[idx];
	}
	T operator[](size_t idx) const
	{
		return vals[idx];
	}
	inline constexpr auto operator==(const Point<T, size>& other) const noexcept
	{
		for(size_t i = 0; i < size; ++i)
		{
			if(vals[i] != other[i])
			{
				return false;
			}
		}
		return true;
	}
	inline constexpr Point operator*(const Point<T, size>& other) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] * other[i];
		}
		return Point(new_vals);
	}
	inline constexpr Point operator/(const Point<T, size>& other) const
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] / other[i];
		}
		return Point(new_vals);
	}
	inline constexpr Point operator*(const double scalar) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] * scalar;
		}
		return Point(new_vals);
	}
	inline constexpr Point& operator+=(const Point<T, size>& other) noexcept
	{
		for(size_t i = 0; i < size; ++i)
		{
			vals[i] += other[i];
		}
		return *this;
	}
	inline constexpr Point operator+(const Point<T, size>& other) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] + other[i];
		}
		return Point(new_vals);
	}
	inline constexpr Point operator+(const T other) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] + other;
		}
		return Point(new_vals);
	}
	inline constexpr Point& operator-() noexcept
	{
		for(size_t i = 0; i < size; ++i)
		{
			vals[i] *= -1;
		}
		return *this;
	};
	inline constexpr Point operator-(const T other) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] - other;
		}
		return Point(new_vals);
	}
	inline constexpr Point operator-(const Point<T, size>& other) const noexcept
	{
		std::array<T, size> new_vals;
		for(size_t i = 0; i < size; ++i)
		{
			new_vals[i] = vals[i] - other[i];
		}
		return Point(new_vals);
	}
	inline std::string toString() const
	{ 
		std::string point_text("[");
		std::for_each(vals.begin(), vals.end()-1, [&](const T& val){ point_text += std::to_string(val) + ", "; });
		point_text += std::to_string(*(vals.end()-1));
		point_text += "]";
		return point_text;
	};

	inline friend std::ostream& operator<<(std::ostream& os, const Point<T, size>& point)
	{
		return (os << point.toString());
	}
};

template<typename T>
class Point2D : public Point<T, 2>
{
public:
	T& x;
	T& y;


public:
	[[nodiscard]] constexpr Point2D() noexcept : 
		Point<T, 2>(), x(this->vals[0]), y(this->vals[1])
	{
	}
	[[nodiscard]] constexpr Point2D(const Point<T, 2>& p) noexcept : 
		Point<T, 2>(p), x(this->vals[0]), y(this->vals[1])
	{
	}
	[[nodiscard]] constexpr Point2D(const T x, const T y) noexcept : 
		Point<T, 2>({x, y}), x(this->vals[0]), y(this->vals[1])
	{
	}
	[[nodiscard]] constexpr Point2D(const std::array<T, 2>& vals) noexcept : 
		Point<T, 2>({vals[0], vals[1]}), x(this->vals[0]), y(this->vals[1])
	{
	}
	~Point2D() = default;

	constexpr Point2D(const Point2D& new_point) noexcept : Point2D(new_point.vals) {}
	constexpr Point2D(Point2D&& new_point) noexcept : Point2D(new_point.vals) {}

	constexpr Point2D& operator=(const Point2D<T>& new_point)
	{
		this->vals = new_point.vals;
		x = this->vals[0];
		y = this->vals[1];
		return *this;
	};
	constexpr Point2D& operator=(Point2D<T>&& new_point)
	{
		this->vals = new_point.vals;
		x = this->vals[0];
		y = this->vals[1];
		return *this;
	};

};

template<typename T>
class Point3D : public Point<T, 3>
{
public:
	T& x;
	T& y;
	T& z;


public:
	[[nodiscard]] constexpr Point3D() noexcept : 
		Point<T, 3>(), x(this->vals[0]), y(this->vals[1]), z(this->vals[2])
	{
	}
	[[nodiscard]] constexpr Point3D(const Point<T, 3>& p) noexcept : 
		Point<T, 3>(p), x(this->vals[0]), y(this->vals[1]), z(this->vals[2])
	{
	}
	[[nodiscard]] constexpr Point3D(const T x, const T y) noexcept : 
		Point<T, 3>({x, y}), x(this->vals[0]), y(this->vals[1]), z(this->vals[2])
	{
	}
	[[nodiscard]] constexpr Point3D(const std::array<T, 3>& vals) noexcept : 
		Point<T, 3>({vals[0], vals[1]}), x(this->vals[0]), y(this->vals[1]), z(this->vals[2])
	{
	}
	~Point3D() = default;

	constexpr Point3D(const Point3D& new_point) noexcept : Point3D(new_point.vals) {}
	constexpr Point3D(Point3D&& new_point) noexcept : Point3D(new_point.vals) {}
	constexpr Point3D& operator=(const Point3D<T>& new_point)
	{
		this->vals = new_point.vals;
		x = this->vals[0];
		y = this->vals[1];
		z = this->vals[2];
		return *this;
	};
	constexpr Point3D& operator=(Point3D<T>&& new_point)
	{
		this->vals = new_point.vals;
		x = this->vals[0];
		y = this->vals[1];
		z = this->vals[2];
		return *this;
	};

};
};// namespace ruff
