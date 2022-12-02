#pragma once
#include <ruff/imgproc/image.hpp>

namespace ruff::imgproc
{
    class ImageAdapter
    {
    protected:
        Image& m_img;
    public:
        ImageAdapter(Image& img) : m_img(img) {};
    };
}