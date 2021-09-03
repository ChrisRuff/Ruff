#include "sand.hpp"

void SandEngine::onCreate()
{
	// Resource dir is defined in root CMakeLists.txt
	const auto font_path = 
		std::filesystem::path(DATA_DIR) / "DejaVuSans.ttf";
	types.push_back(addButton(getWidth()-15, 5, 10, 5, 
				ruff::ui::DARK_YELLOW, pixelRatio, font_path, "Sand"));
	types.push_back(addButton(getWidth()-15, 15, 10, 5, 
				ruff::ui::DARK_BLUE, pixelRatio, font_path, "Water"));
}

void SandEngine::onUpdate(const double deltaTime)
{
	for(int i : types)
	{
		if(buttons[i]->isPressed())
		{
			if(buttons[i]->isChanged())
			{
				selected_type = i;
			}
			else if(selected_type != i)
			{
				buttons[i]->press();
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
