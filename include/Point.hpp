#include <ostream>
namespace Ruff
{
	template<typename T>
	class Point2D
	{
	public:
		T x;
		T y;


	public:
		Point2D(T x, T y) : x(x), y(y)
		{
			
		}
		Point2D() : x(), y()
		{}


		inline friend std::ostream& operator<<(std::ostream &os, const Point2D<T> &p)
		{
			return (os << "[" << p.x << ", " << p.y << "]" << std::endl);
		}
		inline friend bool operator==(const Point2D<T> &l, const Point2D<T>& r)
		{
			return (l.x == r.x && l.y == r.y);
		}
		inline friend bool operator!=(const Point2D<T> &l, const Point2D<T>& r)
		{
			return !(l.x == r.x && l.y == r.y);
		}
	};
};
