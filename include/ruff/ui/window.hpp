#include <memory>

#include "ruff/ui/renderer.hpp"
#include "ruff/ui/image.hpp"

namespace ruff
{
namespace ui
{
	class Window
	{
	private:
		Image frame;
		Pixel bg;
		std::string title{ "Ruff UI Engine" };

	public:
		std::unique_ptr<SDL2_Renderer> renderer;

	public:
		Window(const uint16_t width,
		       const uint16_t height,
		       const Pixel& bg)
		  : frame(width, height, bg), bg(bg),
		    renderer(std::make_unique<SDL2_Renderer>(
		      Point2D<uint16_t>{ width, height }, title))
		{
		}

		void draw(uint16_t x, uint16_t y, const Pixel p)
		{
			Pixel old_p = frame.get(x, y);
			Pixel new_p = Pixel::combine(old_p, p);
			frame.set(x, y, new_p);
		}
		Pixel get(uint16_t x, uint16_t y) { return frame.get(x, y); }


		void push_to_screen() { renderer->render(&frame); }

		void clear() { frame.clear(bg); }
		void setBackground(const Pixel& p)
		{
			this->bg = p;
		}
		void resize(Point2D<uint16_t> new_size)
		{
			frame = Image(new_size.x, new_size.y, bg);
			renderer->resize(new_size);
		}
		void setTitle(const std::string& title)
		{
			this->title = title;
			renderer->setTitle(title);
		}
		Point2D<uint16_t> size() const
		{
			return { frame.width(), frame.height() };
		}
	};
};// namespace ui
};// namespace ruff
