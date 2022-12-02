#include <memory>

#include <ruff/imgproc/image.hpp>
#include <ruff/ui/renderer.hpp>

namespace ruff
{
namespace ui
{
	class Window
	{
	private:
		imgproc::Image frame;
		imgproc::Pixel bg;
		std::string title{ "Ruff UI Engine" };

		size_t ratio{ 1 };

	public:
		std::unique_ptr<SDL2_Renderer> renderer;

	public:
		Window(const uint16_t width,
		       const uint16_t height,
		       const imgproc::Pixel& bg)
		  : frame(width, height, bg), bg(bg),
		    renderer(std::make_unique<SDL2_Renderer>(
		      Point2D<uint16_t>{ width, height }, title))
		{
		}

		void draw(uint16_t x, uint16_t y, const imgproc::Pixel& p)
		{
			const imgproc::Pixel& old_p = frame.Get(x, y);
			const imgproc::Pixel& new_p = imgproc::Pixel::combine(old_p, p);
			frame.Set(x, y, new_p);
		}
		imgproc::Pixel get(uint16_t x, uint16_t y) const { return frame.Get(x, y); }


		void push_to_screen() { renderer->render(&frame); }

		void clear() { frame.Clear(bg); }
		void setBackground(const imgproc::Pixel& p)
		{
			this->bg = p;
		}
		void resize(Point2D<uint16_t> new_size)
		{
			new_size /= ratio;
			frame = imgproc::Image(new_size.x, new_size.y, bg);
			renderer->resize(new_size);
		}
		void setTitle(const std::string& title)
		{
			this->title = title;
			renderer->setTitle(title);
		}
		Point2D<uint16_t> size() const { return { frame.Width(), frame.Height() }; };
		void setRatio(const size_t pr)
		{
			this->ratio = pr;
			resize({ static_cast<unsigned short>(frame.Width() / pr),
			         static_cast<unsigned short>(frame.Height() / pr) });
		}
		size_t getRatio() const { return this->ratio; };
	};
};// namespace ui
};// namespace ruff
