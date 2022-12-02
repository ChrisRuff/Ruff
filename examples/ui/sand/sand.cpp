#include "sand.hpp"

void SandEngine::onCreate()
{
	screen->setRatio(3);
	// Resource dir is defined in root
	// CMakeLists.txt
	const auto font_path =
	  std::filesystem::path(DATA_DIR) / "DejaVuSans.ttf";
	ruff::ui::Button* sand =
	  addButton({ static_cast<unsigned short>(getWidth() - 30), 5 },
	            { 20, 10 });
	sand->setColor(ruff::imgproc::DARK_YELLOW);

	ruff::ui::Button* water =
	  addButton({ static_cast<unsigned short>(getWidth() - 30), 25 },
	            { 20, 10 });
	water->setColor(ruff::imgproc::DARK_BLUE);
}
void SandEngine::onResize()
{
	buttons.clear();
	blocks.clear();
	onCreate();
}

void SandEngine::onUpdate(double deltaTime)
{
	deltaTime *= 100;
	selected_type = -1;
	for(size_t i = 0; i < buttons.size(); ++i)
	{
		if(buttons[i]->isPressed() && selected_type == -1)
		{
			selected_type = i;
		}
		else if(buttons[i]->isPressed())
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
