#include <ruff/imgproc/image_drawer.hpp>
namespace ruff::imgproc
{
    void ImageDrawer::DrawLine(const Point2D<uint16_t>& p1,
                     const Point2D<uint16_t>& p2,
                     const Pixel& color)
    {
        bool steep = false;
        Point2D<uint16_t> start(p1);
        Point2D<uint16_t> end(p2);
        if(std::abs(start.x - end.x) < std::abs(start.y - end.y))
        {
            std::swap(start.x, start.y);
            std::swap(end.x, end.y);
        }
        if(start.x > end.x)
        {
            std::swap(start.x, end.x);
            std::swap(start.y, end.y);
        }
        int dx = end.x - start.x;
        int dy = end.y - start.y;
        int d_error = std::abs(dy) * 2;
        int error = 0;
        int y = 0;

        const int y_inc = (end.y > start.y ? 1 : -1);
        for(uint16_t x = start.x; x <= end.x; ++x)
        {
            if(steep) { m_img.Set(y, x, color); }
            else
            {
                m_img.Set(x, y, color);
            }
            error += d_error;
            if(error > dx)
            {
                y += y_inc;
                error -= dx * 2;
            }
        }
    }
    void ImageDrawer::DrawRectangle(const Point2D<uint16_t>& p1,
                          const Point2D<uint16_t>& p2,
                          const Pixel& color)
    {
        uint16_t min_x = std::min(p1.x, p2.x);
        uint16_t min_y = std::min(p1.y, p2.y);
        uint16_t max_x = std::max(p1.x, p2.x);
        uint16_t max_y = std::max(p1.y, p2.y);
        for(uint16_t i = min_x; i < max_x; ++i)
        {
            for(uint16_t j = min_y; j < max_y; ++j) { m_img.Set(i, j, color); }
        }
    }
    void ImageDrawer::OverlayImage(const Image& overlay, uint16_t x, uint16_t y)
    {
        for(uint16_t i = x; i < overlay.Width() + x && i < m_img.Width(); ++i)
        {
            for(uint16_t j = y; j < overlay.Height() + y && j < m_img.Height(); ++j)
            {
                m_img.Set(i, j, overlay.Get(i - x, j - y));
            }
        }
    }
}
