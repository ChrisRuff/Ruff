#include <ostream>
namespace MLibs
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
		Point2D() {}
	};
	template <typename T>
	inline std::ostream& operator<<(std::ostream &os, const Point2D<T> &p)
	{
		return (os << "[" << p.x << ", " << p.y << "]" << std::endl);
	}
};
