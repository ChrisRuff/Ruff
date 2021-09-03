#include <ruff/core/asserts.hpp>
#include "ruff/ui/ui.hpp"
#include "ruff/ui/pixel.hpp"

namespace ruff
{
namespace ui
{
	struct Tile
	{
		bool wall{false};
		Pixel shade{ruff::ui::WHITE};

		operator bool()
		{
			return wall;
		}
	};
	struct Player
	{
		// Player traits
		Point2D<float> pos{0,0};
		double angle{0};
		double fov{M_PI/3};
	};
	class Map
	{
	public:
		const Point2D<uint16_t> map_size;
		const Point2D<uint16_t> cell_size;
		std::vector<Tile> game_map{};

		const uint8_t rows;
		const uint8_t cols;

	public:
		Map(const Point2D<uint16_t>& map_size, 
				const Point2D<uint16_t>& num_cells) :
			map_size(map_size), cell_size(map_size/num_cells), 
			rows(num_cells.x), cols(num_cells.y)
		{
			ruff_assert(map_size[0] % num_cells[0] == 0, 
					"Map size must be a multiple of cell size");
			ruff_assert(map_size[1] % num_cells[1] == 0, 
					"Map size must be a multiple of cell size");

			game_map.resize(rows * cols);
		}
		[[nodiscard]] Tile at(const uint16_t x, const uint16_t y) const
		{
			return game_map[x*cols + y];
		}
		[[nodiscard]] Tile& at(const uint16_t x, const uint16_t y) 
		{
			return game_map[x*cols + y];
		}
	};

	template<
		size_t map_width, size_t map_height,
		size_t num_cells_x, size_t num_cells_y>
	class RayEngine : public Engine
	{
	public:
		Map tile_map;
		Player player{};
		const double move_scale{0.1};
		const double turn_scale{0.01};

		std::vector<Point2D<uint16_t>> hits{};
		const uint16_t mid_point = getHeight()/2;


	public:
		RayEngine(const uint16_t width, const uint16_t height, 
				const float px=map_width/2, const float py=map_height/2,
				const std::string& title = "Ray Engine", 
				const int pixelRatio = 5) 
			: ruff::ui::Engine(height, width, title, pixelRatio), 
				tile_map(
					{map_width,    map_height},
					{num_cells_x, num_cells_y}),
				player({px,py})
		{}

		void drawMap(const Point2D<uint16_t>& top_left)
		{
			for(uint16_t i = 0; i < tile_map.cols; ++i)
			{
				for(uint16_t j = 0; j < tile_map.rows; ++j)
				{
					const Point2D<uint16_t> tl{
						static_cast<uint16_t>(top_left.x+i*tile_map.cell_size.x), 
						static_cast<uint16_t>(top_left.y+j*tile_map.cell_size.y)};

					const Point2D<uint16_t> br = tl + tile_map.cell_size;
					if(tile_map.at(i, j))
					{
						drawSquare(tl, br,
									ruff::ui::RED, true);
					}
					else
					{
						drawSquare(tl, br,
									ruff::ui::DARK_GREY, true);
					}
				}
			}
			for(size_t i = 0; i < hits.size(); ++i)
			{
				ruff::log( player.pos.toString() + " " + 
						hits[i].toString() + " " +
						std::to_string(player.angle));
				Point2D<uint16_t> start{
						static_cast<uint16_t>(player.pos.x), 
						static_cast<uint16_t>(player.pos.y)};
				drawLine(start, hits[i], ruff::ui::WHITE, 1);
			}
		}
		virtual void onCreate() override {}
		virtual void onUpdate(double deltaTime) override
		{
			if(Engine::keys[SDLK_a])
			{
				player.angle -= turn_scale * deltaTime;
			}
			if(Engine::keys[SDLK_d])
			{
				player.angle += turn_scale * deltaTime;
			}
			if(Engine::keys[SDLK_w])
			{
				const float new_x = player.pos.x +
					(deltaTime*std::cos(player.angle-(player.fov/2))*(move_scale));
				const float new_y = player.pos.y + 
					(deltaTime*std::sin(player.angle-(player.fov/2))*(move_scale));
				if(!tile_map.at(new_x, new_y))
				{
					player.pos.x = new_x;
					player.pos.y = new_y;
				}
			}
			if(Engine::keys[SDLK_s])
			{
				const float new_x = player.pos.x - 
					(deltaTime*std::cos(player.angle-(player.fov/2))*move_scale);
				const float new_y = player.pos.y - 
					(deltaTime*std::sin(player.angle-(player.fov/2))*move_scale);

				if(!tile_map.at(new_x, new_y))
				{
					player.pos.x = new_x;
					player.pos.y = new_y;
				}
			}

			Engine::clearScreen(VERY_DARK_GREY);

			hits.clear();
			for(size_t i = 0; i < Engine::getWidth(); ++i)
			{
				float angle = (player.angle-player.fov) + 
					(player.fov*i)/Engine::getWidth();

				// DDA algorithm 
				// https://www.youtube.com/watch?v=NbSee-XM7WA
				Point2D<float> ray_dir(std::cos(angle), std::sin(angle));
				Point2D<float> ray_step{
					std::sqrt(1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)), 
					std::sqrt(1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y))};

				Point2D<int> check_point{
					static_cast<int>(player.pos.x), 
					static_cast<int>(player.pos.y)};
				Point2D<int> step;
				Point2D<float> ray_length;

				if(ray_dir.x < 0)
				{
					step.x = -1;
					ray_length.x = (player.pos.x - static_cast<float>(check_point.x)) * ray_step.x;
				}
				else
				{
					step.x = 1;
					ray_length.x = (static_cast<float>(check_point.x+1)-player.pos.x) * ray_step.x;
				}
				if(ray_dir.y < 0)
				{
					step.y = -1;
					ray_length.y = (player.pos.y - static_cast<float>(check_point.y)) * ray_step.y;
				}
				else
				{
					step.y = 1;
					ray_length.y = (static_cast<float>(check_point.y+1)-player.pos.y) * ray_step.y;
				}

				bool tile_found = false;
				float max_dist = 1000.0f;
				float dist = 0.0f;
				while(!tile_found && dist < max_dist)
				{
					if(ray_length.x < ray_length.y)
					{
						check_point.x += step.x;
						dist = ray_length.x;
						ray_length.x += ray_step.x;
					}
					else
					{
						check_point.y += step.y;
						dist = ray_length.y;
						ray_length.y += ray_step.y;
					}

					if(check_point.x >= 0 && 
							check_point.y >= 0 && 
							check_point.x < static_cast<float>(map_width) && 
							check_point.y < static_cast<float>(map_height))
					{
						if(tile_map.at(check_point.x, check_point.y))
						{
							tile_found = true;
						}
					}
				}

				Point2D<float> hit;
				if(tile_found)
				{
					hit = player.pos + (ray_dir * dist);
					hits.emplace_back(hit.x, hit.y);
				}


				uint16_t height = (Engine::getHeight() / 
						(dist*std::cos(angle-player.angle)));

				height = std::min(height, mid_point);

				// Get texture column

				for(uint16_t j = mid_point-height; j < mid_point+height; ++j)
				{
					const ruff::ui::Pixel pixel = ruff::ui::RED;
					draw(i, j, pixel);
				}
			}
			drawMap({0,0});
		};

		RayEngine(const RayEngine&) = delete;
		RayEngine(const RayEngine&&) = delete;
		RayEngine& operator=(const RayEngine&&) = delete;
		RayEngine& operator=(const RayEngine&) = delete;
		~RayEngine() override = default;
	};
};
};
