#include "sand.hpp"

void SandEngine::onCreate()
{
	// Resource dir is defined in root
	// CMakeLists.txt
	const auto font_path =
	  std::filesystem::path(DATA_DIR) / "DejaVuSans.ttf";
	ruff::ui::Button* sand =
	  addButton({ static_cast<unsigned short>(getWidth() - 50), 5 },
	            { 50, 30 });
	sand->setColor(ruff::ui::DARK_YELLOW);

	ruff::ui::Button* water =
	  addButton({ static_cast<unsigned short>(getWidth() - 50), 40 },
	            { 50, 30 });
	water->setColor(ruff::ui::DARK_BLUE);
}
void SandEngine::onResize()
{
	buttons.clear();
	blocks.clear();
	onCreate();
}

void SandEngine::onUpdate(const double deltaTime)
{
	selected_type = -1;
	for(size_t i = 0; i < buttons.size(); ++i)
	{
		if(buttons[i]->isPressed())
		{
			buttons[i]->press();
		}
	}

	if(mouse.mouse_held[0])
	{
		if(!onButton(mouse))// Don't generate on
		                    // buttons
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

	for(auto& b : blocks)
	{
		const ruff::Point2D displacement = b->getVelocity() * deltaTime;
		ruff::Point2D pos(
		  static_cast<uint16_t>(displacement.x + b->getPosition().x),
		  static_cast<uint16_t>(displacement.y + b->getPosition().y));

		// b->setPosition(pos);
		b->setVelocity(b->getVelocity()
		               + b->getAcceleration() * deltaTime);
		b->update(deltaTime);
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
