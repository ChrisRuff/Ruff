#include <ruff/imgproc/image_manipulator.hpp>

namespace ruff::imgproc
{

    void ImageManipulator::Resize(uint16_t new_w,
                       uint16_t new_h,
                       RESAMP_METHOD method)
    {
        (void)new_w;
        (void)new_h;
        (void)method;
    }

    Image ImageManipulator::Rotate(const double theta,
                        ROTATION_METHOD rotation_method) const
    {
		    (void)rotation_method;
        const auto h = m_img.Height();
        const auto w = m_img.Width();

        const auto sin_val = std::sin(theta);
        const auto cos_val = std::cos(theta);

        // https://gautamnagrawal.medium.com/rotating-image-by-any-angle-shear-transformation-using-only-numpy-d28d16eb5076
        const Point2D<uint16_t> new_dims(
		    std::round(std::abs(w * cos_val) + std::abs(h * sin_val)) + 1,
		    std::round(std::abs(h * cos_val) + std::abs(w * sin_val)) + 1);

				Image rotated(new_dims.x, new_dims.y, WHITE);

				const Point2D<uint16_t> center(std::round(((w + 1) / 2) - 1),
																			 std::round(((h + 1) / 2) - 1));
				const Point2D<uint16_t> new_center(
					std::round(((new_dims.x + 1.0) / 2) - 1),
					std::round(((new_dims.y + 1.0) / 2) - 1));

				for(uint16_t i = 0; i < h; ++i)
				{
					  for(uint16_t j = 0; j < w; ++j)
					  {
					  	  const auto x = w - 1 - j - center.x;
					  	  const auto y = h - 1 - i - center.y;

					  	  const auto new_x =
					  	  	new_center.x
					  	  	- std::round(x * cos_val + y * sin_val);
					  	  const auto new_y =
					  	  	new_center.y
					  	  	- std::round(-x * sin_val + y * cos_val);

					  	  if(0 <= new_x && new_x < new_dims.x &&
					  	  					 0 <= new_y && new_y < new_dims.y)
					  	  {
								    const auto pixel = m_img.Get(j, i);
					  	  	  rotated.Set(new_x, new_y, pixel);
					  	  }
					  }
				}
				return rotated;
    }
}
