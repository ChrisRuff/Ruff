// STL Packages
#include <cmath>
#include <memory>
#include <unordered_map>
#include <random>
#include <filesystem>

// Source
#include "ruff/ui/ui.hpp"

using sint = short int;

struct Tree
{
	ruff::Point2D<int> pos{};
	double angle{};
	int width{};
	int height{};
	Tree(ruff::Point2D<int> pos, double angle, int width, int height) noexcept : pos(pos), angle(angle), width(width), height(height) {}

	bool operator<(const Tree& other) const
	{
		return (height - (angle / 10) < other.height - (other.angle / 10));
	}
};
class Sprites : public ruff::ui::Engine
{
private:
	ruff::ui::Image tree{};
	ruff::ui::Image stump{};
	ruff::ui::Image shadow{};
	std::vector<std::unordered_map<std::string, int>> spriteInfo{};
	std::default_random_engine generator{ std::random_device{}() };
	std::uniform_real_distribution<double> random{};

	std::vector<Tree> trees{};

public:
	Sprites(const sint width, const sint height, std::string title = "Sprite Engine", int pixelRatio = 1)
	  : Engine(height, width, std::move(title), pixelRatio) {}

	Sprites(const Sprites& other) = delete;
	Sprites(const Sprites&& other) = delete;
	Sprites& operator=(const Sprites&) = delete;
	Sprites& operator=(Sprites&&) = delete;

	~Sprites() override = default;


	void onCreate() override
	{
		random = std::uniform_real_distribution<double>(0.1, 1.5);

		// Resource dir is defined in root CMakeLists.txt
		const auto resources = std::filesystem::path(DATA_DIR);

		tree = ruff::ui::Image::read(resources / "tree.png");
		stump = ruff::ui::Image::read(resources / "stump.png");
		shadow = ruff::ui::Image::read(resources / "shadow.png");

		auto pos = ruff::Point2D<int>{ getWidth() / 2, getHeight() / 2 };
		trees.emplace_back(pos, 0, tree.width(), tree.height());
		pos = ruff::Point2D<int>{ rand() % getWidth(), rand() % getHeight() };
		trees.emplace_back(pos, 0, tree.width(), tree.height());
		pos = ruff::Point2D<int>{ rand() % getWidth(), rand() % getHeight() };
		trees.emplace_back(pos, 0, tree.width(), tree.height());
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
		for(auto& t : trees)
		{
			displayImage(stump, t.pos.x, t.pos.y);
		}
		// Draw all the trees
		for(auto& t : trees)
		{
			displayImage(tree, static_cast<uint16_t>(t.pos.x + 3 + (t.angle / 10)), 
					static_cast<uint16_t>(t.pos.y - 100 - (t.angle / 10)), t.angle);
		}
		// Draw all the trees
		int size = trees.size();
		for(int i = 0; i < size; ++i)
		{
			Tree& t = trees[i];
			if(t.angle < 90.0)
			{
				t.angle += random(generator) * deltaTime;
			}
			else
			{
				t.angle = 0;
				auto pos = ruff::Point2D<int>{ rand() % getWidth(), rand() % getHeight() };
				trees.emplace_back(pos, 0, tree.width(), tree.height());
			}
		}
		std::sort(trees.begin(), trees.end());
	}
};

int main()
{
	constexpr int width = 1000;
	constexpr int height = 1200;
	constexpr int pixelWidth = 1;

	Sprites spriteEngine(width, height, "Sprites", pixelWidth);
	spriteEngine.launch();
	return 0;
}
