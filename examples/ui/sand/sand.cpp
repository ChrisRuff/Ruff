#include "sand.h"


void SandEngine::onCreate()
{
	types.push_back(addButton(getWidth()-15, 5, 10, 5, ruff::ui::DARK_YELLOW, pixelRatio, 
				"../examples/ui/astar/DejaVuSans.ttf", "Sand"));
	types.push_back(addButton(getWidth()-15, 15, 10, 5, ruff::ui::DARK_BLUE, pixelRatio, 
				"../examples/ui/astar/DejaVuSans.ttf", "Water"));
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
		b->update(deltaTime, this);
		draw(b->getPosition(), b->getColor());
	}
	usleep(1000);
}

int main()
{
	// Create game engine and then run it
	SandEngine engine(width, height);
	engine.launch();
	return 0;
}
