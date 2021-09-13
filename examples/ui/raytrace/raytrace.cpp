#include <ruff/ui/ray-engine.hpp>

template<
	size_t map_width, size_t map_height, 
	size_t num_cells_x, size_t num_cells_y
>
class Ray : public ruff::ui::RayEngine<map_width, map_height, num_cells_x, num_cells_y>
{
public:
	Ray(const uint16_t width, const uint16_t height) :
			ruff::ui::RayEngine<map_width, map_height, num_cells_x, num_cells_y>(
					width, height, 20, 20)
	{
	}

	virtual void onCreate() override
	{
		for(size_t i = 0; i < num_cells_x; ++i)
		{
			ruff::ui::RayEngine<
				map_width,
				map_height,
				num_cells_x,
				num_cells_y>::tile_map.at(i, 0).wall = true;

			ruff::ui::RayEngine<
				map_width,
				map_height,
				num_cells_x,
				num_cells_y>::tile_map.at(i, num_cells_x-1).wall = true;
		}
		for(size_t j = 0; j < num_cells_y; ++j)
		{
			ruff::ui::RayEngine<
				map_width,
				map_height,
				num_cells_x,
				num_cells_y>::tile_map.at(0, j).wall = true;

			ruff::ui::RayEngine<
				map_width,
				map_height,
				num_cells_x,
				num_cells_y>::tile_map.at(num_cells_y-1, j).wall = true;
		}

		ruff::ui::RayEngine<
			map_width,
			map_height,
			num_cells_x,
			num_cells_y>::tile_map.at(num_cells_x/2,num_cells_y/2).wall = true;
	}

	virtual void onUpdate(double deltaTime) override
	{
		ruff::ui::RayEngine<
			map_width,
			map_height, 
			num_cells_x, 
			num_cells_y>::onUpdate(deltaTime);
	}
};
int main()
{
	Ray<50,50,50,50> engine(800, 1200);
	engine.launch();
}
