#pragma once

#include <ruff/imgproc/image.hpp>
#include <ruff/imgproc/adapter.hpp>
namespace ruff::imgproc
{
    class ImageDrawer : public ImageAdapter
    {
    public:
        ImageDrawer(Image& img) : ImageAdapter(img) {}

        void DrawLine(const Point2D<uint16_t>& p1,
                                   const Point2D<uint16_t>& p2,
                                   const Pixel& color);
        void DrawRectangle(const Point2D<uint16_t>& p1,
                                  const Point2D<uint16_t>& p2,
                                  const Pixel& color);
        void OverlayImage(const Image& overlay, uint16_t x, uint16_t y);
    };
}
