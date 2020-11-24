// STL Packages
#include <cmath>
#include <memory>
#include <unordered_map>
#include <random>

// Source
#include "ui/ui.hpp"

using sint = short int;

struct Tree
{
	ruff::Point2D<int> pos{};
	double angle{};
	int width{};
	int height{};
	Tree(ruff::Point2D<int> pos, double angle, int width, int height) noexcept :
		pos(pos), angle(angle) , width(width), height(height) {}

	bool operator < (const Tree& other) const
	{
		return (height-(angle/10) < other.height-(other.angle/10));
	}

};
class Sprites : public ruff::ui::Engine
{
private:
	int tree{-1};
	int stump{-1};
	int shadow{-1};
	std::vector<std::unordered_map<std::string, int>> spriteInfo{};
	std::default_random_engine generator{std::random_device{}()};
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

		tree = loadSprite("../examples/ui/sprites/tree.bmp");
		stump = loadSprite("../examples/ui/sprites/stump.bmp");
		shadow = loadSprite("../examples/ui/sprites/shadow.bmp");

		spriteInfo.push_back(getSpriteInfo(tree));
		spriteInfo.push_back(getSpriteInfo(stump));
		spriteInfo.push_back(getSpriteInfo(shadow));

		auto pos = ruff::Point2D<int>{getWidth()/2, getHeight()/2};
		trees.emplace_back(pos, 0, spriteInfo[tree]["width"], spriteInfo[tree]["height"]);
		pos = ruff::Point2D<int>{rand()%getWidth(), rand()%getHeight()};
		trees.emplace_back(pos, 0, spriteInfo[tree]["width"], spriteInfo[tree]["height"]);
		pos = ruff::Point2D<int>{rand()%getWidth(), rand()%getHeight()};
		trees.emplace_back(pos, 0, spriteInfo[tree]["width"], spriteInfo[tree]["height"]);
	}
	void onUpdate(double deltaTime) override 
	{

		clearScreen();

		// Draw all the shadows
		for(auto& t : trees)
		{
			displaySprite(t.pos.x+(t.angle*1.6), t.pos.y+10, shadow, 1);
		}
		// Draw all the stumps
		for(auto& t : trees)
		{
			displaySprite(t.pos.x, t.pos.y, stump, 1);
		}
		for(auto& t : trees)
		{
			displaySprite(t.pos.x+3+(t.angle/10), t.pos.y-100-(t.angle/10), tree, 1, t.angle, 
					t.width/2, t.height);
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
				auto pos = ruff::Point2D<int>{rand()%getWidth(), rand()%getHeight()};
				trees.emplace_back(pos, 0, spriteInfo[tree]["width"], spriteInfo[tree]["height"]);
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

	Sprites circleEngine(width, height, "Sprites", pixelWidth);
	circleEngine.launch();
	return 0;
}
