#pragma once
#include <ruff/imgproc/image.hpp>
#include <ruff/imgproc/adapter.hpp>

namespace ruff::imgproc
{
    enum class RESAMP_METHOD
    {
        NEAREST_NEIGHBOR
    };
    enum class ROTATION_METHOD
    {
        PARTIAL_SHEAR,
        SHEAR
    };
    class ImageManipulator : public ImageAdapter
    {
    public:
        ImageManipulator(Image& img) : ImageAdapter(img) {}

        Image Rotate(const double theta,
                     const ROTATION_METHOD rotation_method =
                     ROTATION_METHOD::SHEAR) const;
        void Resize(
                uint16_t new_w,
                uint16_t new_h,
                const RESAMP_METHOD method = RESAMP_METHOD::NEAREST_NEIGHBOR);
    };
}
