#include "ruff/ui/ui.hpp"
#include "ruff/ui/pixel.hpp"

namespace ruff
{
namespace ui
{
	struct Tile
	{
		bool wall{false};

		ruff::ui::Pixel shade{ruff::ui::WHITE};
	};
	class Map
	{
		const std::array<uint16_t, 2> map_size;
		const std::array<uint16_t, 2> cell_size;
		const std::vector<Tile> game_map{};

		// Player traits
		Point2D<uint16_t> player_pos;
		const double fov;

		const uint8_t rows;
		const uint8_t cols;

		Map(const std::array<uint16_t,2>& map_size, 
				const std::array<uint16_t,2>& num_cells,
				Point2D<uint16_t>& player_pos, 
				const double fov = M_PI/4) :
			map_size(map_size), cell_size(cell_size), 
			player_pos(player_pos), fov(fov), 
			rows(map_size[0]/num_cells[0]), cols(map_size[1]/num_cells[1])
		{
			ruff_assert(map_size[0] % num_cells[0] == 0, 
					"Map size must be a multiple of cell size");
			ruff_assert(map_size[1] % num_cells[1] == 0, 
					"Map size must be a multiple of cell size");

			game_map.resize(rows * cols);
		}
	};

	class RayEngine : public ruff::ui::Engine
	{
	private:
		Map tile_map;

	public:
		RayEngine(const int16_t width, const int16_t height, 
				const std::string& title = "Ray Engine", 
				const int pixelRatio = 8) 
			: ruff::ui::Engine(height, width, title, pixelRatio) {}

		~RayEngine() override = default;

		RayEngine(const RayEngine&) = delete;
		RayEngine(const RayEngine&&) = delete;
		RayEngine& operator=(const RayEngine&&) = delete;
		RayEngine& operator=(const RayEngine&) = delete;

		void onCreate() override;
		void onUpdate(double deltaTime) override;
	};
};
};
