// STL Packages
#include <cmath>
#include <memory>
#include <unordered_map>
#include <random>
#include <filesystem>

// Source
#include <ruff/imgproc/image_io.hpp>
#include <ruff/ui/ui.hpp>

using sint = short int;

struct Tree
{
	ruff::Point2D<int> pos{};
	double angle{};
	int width{};
	int height{};
	Tree(ruff::Point2D<int> pos,
	     double angle,
	     int width,
	     int height) noexcept
	  : pos(pos), angle(angle), width(width), height(height)
	{
	}

	bool operator<(const Tree& other) const
	{
		return (height - (angle / 10)
		        < other.height - (other.angle / 10));
	}
};
class Sprites : public ruff::ui::Engine
{
private:
    // Resource dir is defined in root
    // CMakeLists.txt
	ruff::imgproc::Image tree = ruff::imgproc::ImageIO::read(std::filesystem::path(DATA_DIR) / "tree.png");
	ruff::imgproc::Image stump = ruff::imgproc::ImageIO::read(std::filesystem::path(DATA_DIR) / "stump.png");
	ruff::imgproc::Image shadow = ruff::imgproc::ImageIO::read(std::filesystem::path(DATA_DIR) / "shadow.png");
	std::vector<std::unordered_map<std::string, int>> spriteInfo{};
	std::default_random_engine generator{ std::random_device{}() };
	std::uniform_real_distribution<double> random{};

	std::vector<Tree> trees{};

public:
	Sprites(const sint width, const sint height) : Engine(height, width)
	{

	}

	Sprites(const Sprites& other) = delete;
	Sprites(const Sprites&& other) = delete;
	Sprites& operator=(const Sprites&) = delete;
	Sprites& operator=(Sprites&&) = delete;

	~Sprites() override = default;


	void onCreate() override
	{
		random = std::uniform_real_distribution<double>(0.1, 1.5);

		auto pos = ruff::Point2D<int>{ getWidth() / 2, getHeight() / 2 };
		trees.emplace_back(pos, 0, tree.Width(), tree.Height());
		pos =
		  ruff::Point2D<int>{ rand() % getWidth(), rand() % getHeight() };
		trees.emplace_back(pos, 0, tree.Width(), tree.Height());
		pos =
		  ruff::Point2D<int>{ rand() % getWidth(), rand() % getHeight() };
		trees.emplace_back(pos, 0, tree.Width(), tree.Height());

		screen->setBackground(ruff::imgproc::DARK_BLUE);
	}
	void onUpdate(double deltaTime) override
	{

		clearScreen();

		// Draw all the shadows
		for(auto& t : trees)
		{
			displayImage(shadow,
			             static_cast<uint16_t>(t.pos.x + (t.angle * 1.6)),
			             static_cast<uint16_t>(t.pos.y + 10));
		}
		// Draw all the stumps
		for(auto& t : trees) { displayImage(stump, t.pos.x, t.pos.y); }
		// Draw all the trees
		for(auto& t : trees)
		{
			if(t.angle != -1)
			{
				displayImage(
				  tree,
				  static_cast<uint16_t>(t.pos.x - 50 - (t.angle * 1.5)),
				  static_cast<uint16_t>(t.pos.y - 200 + (t.angle / 2)),
				  -t.angle * (M_PI / 180) + 90);
			}
		}
		// Update all the trees
		int size = trees.size();
		for(int i = 0; i < size; ++i)
		{
			Tree& t = trees[i];
			if(t.angle < 90.0)
			{
				t.angle += random(generator) * deltaTime / 2;
			}
			else if(t.angle > 0)
			{
				t.angle = -1;
				if(rand() % 2)
				{
					auto pos = ruff::Point2D<int>{ rand() % getWidth(),
						                             rand() % getHeight() };
					trees.emplace_back(pos, 0, tree.Width(), tree.Height());
				}
			}
		}
		std::sort(trees.begin(), trees.end());
	}
};

int main()
{
	constexpr int width = 1000;
	constexpr int height = 1200;

	Sprites spriteEngine(width, height);
	spriteEngine.launch();
	return 0;
}
