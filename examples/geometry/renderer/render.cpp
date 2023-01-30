#include <ruff/core/models/point.hpp>
#include <ruff/imgproc/pixel.hpp>
#include <ruff/imgproc/image.hpp>
#include <ruff/geometry/mesh.hpp>

constexpr static int height = 800;
constexpr static int width = 800;
int main()
{
	auto mesh = ruff::geometry::read_obj<double, 3>(
	  std::string(DATA_DIR) + "head.obj");
	if(mesh.nverts() > 0)
	{
		mesh.write_ply(std::string(DATA_DIR) + "head.ply");
		std::cout << "Exported to " << DATA_DIR << "/head.ply"
		          << std::endl;
	}
	else
	{
		std::cout << "Unable to import mesh" << std::endl;
	}
}
