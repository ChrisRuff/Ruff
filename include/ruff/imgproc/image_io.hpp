#pragma once
#include <ruff/imgproc/image.hpp>
#include <ruff/imgproc/adapter.hpp>
namespace ruff::imgproc
{
    class FailWriteException : public std::exception
    {
    private:
        std::string message{ "Could not write file" };

    public:
        FailWriteException() = default;
        FailWriteException(const std::string message) : message(message) {}
        const char* what() const throw() { return message.c_str(); }
    };

    class ImageIO : public ImageAdapter
    {
    public:
        ImageIO(Image& img) : ImageAdapter(img) {}

        bool write(const std::filesystem::path &out_path) const;
        [[nodiscard]] Image static read(const std::filesystem::path &in_path);
    };
}