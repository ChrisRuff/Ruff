#include "sand.hpp"

void SandEngine::onCreate()
{
	// Resource dir is defined in root CMakeLists.txt
	const auto font_path = 
		std::filesystem::path(DATA_DIR) / "DejaVuSans.ttf";
	ruff::ui::Button* sand = addButton({getWidth()-15, 5}, {10, 5});
	sand->setColor(ruff::ui::DARK_YELLOW);
	types.push_back(sand);

	ruff::ui::Button* water = addButton({getWidth()-15, 15}, {10, 5});
	water->setColor(ruff::ui::DARK_BLUE);
	types.push_back(water);
}

void SandEngine::onUpdate(const double deltaTime)
{
	for(size_t i = 0; i < types.size(); ++i)
	{
		if(types[i]->isPressed())
		{
			selected_type = i;
			if(selected_type != i)
			{
				types[i]->press();
			}
		}
	}

	if(mouse.mouse_held[0])
	{
		if(!onButton(mouse)) // Don't generate on buttons
		{
			switch(selected_type)
			{
				case 0: 
					generate<Sand>();
					break;
				case 1:
					generate<Water>();
					break;
			}
		}
	}
	clearScreen();

	for(const auto& b : blocks)
	{
		const ruff::Point2D displacement = b->getVelocity() * deltaTime;
		ruff::Point2D pos(
			static_cast<uint16_t>(displacement.x + b->getPosition().x), 
			static_cast<uint16_t>(displacement.y + b->getPosition().y));

		ruff::logWarning(getPixel(pos));
		if(getPixel(pos) == ruff::ui::BLANK)
		{
			b->setPosition(pos);
			b->setVelocity(b->getVelocity() + b->getAcceleration() * deltaTime);
			b->update(deltaTime);
		}
		draw(b->getPosition(), b->getColor());
	}
}

int main()
{
	// Create game engine and then run it
	SandEngine engine(width, height);
	engine.launch();
	return 0;
}
