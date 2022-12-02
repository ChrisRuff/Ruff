#pragma once
#include <ruff/core/utility.hpp>
#include <ruff/imgproc/adapter.hpp>

#include <string>
namespace ruff::imgproc
{
    class ASCII : public ImageAdapter
    {
        const std::string m_density_map{"@#9876543210?!abc;:+=-,._ "};

        bool m_inverse;
        core::Tensor<char> m_ascii_img;

    public:
        ASCII(Image& img, bool inverse=false) : ImageAdapter(img), m_inverse(inverse),
            m_ascii_img(img.Width(), img.Height())
        {

        }
        void Compute()
        {
            for(size_t i = 0; i < m_img.Width(); ++i)
            {
                for(size_t j = 0; j < m_img.Height(); ++j)
                {
                    const auto& pix = m_img.Get(i, j);
                    uint8_t density = m_inverse ? 1-pix.Magnitude() : pix.Magnitude();
                    uint8_t idx = core::MapToRange(density,
                                                         static_cast<uint8_t>(0),
                                                         static_cast<uint8_t>(255),
                                                         static_cast<uint8_t>(0),
                                                         static_cast<uint8_t>(m_density_map.size()));
                    m_ascii_img(i, j) = m_density_map.at(idx);
                }
            }
        }
        core::Tensor<char> GetData()
        {
            return m_ascii_img;
        }
    };

}