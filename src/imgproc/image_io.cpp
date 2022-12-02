#include <ruff/imgproc/image_io.hpp>

#include "png.h"

namespace ruff::imgproc
{
    bool ImageIO::write(const std::filesystem::path& out_path) const
    {
        if(out_path.extension() == ".png")
        {
            FILE* fp;
            png_structp png;
            png_infop info;
            try
            {
                fp = fopen(out_path.c_str(), "wb");
                if(!fp) throw FailWriteException();
                // https://gist.github.com/niw/5963798
                png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                if(!png) throw FailWriteException();

                info = png_create_info_struct(png);
                if(!info) throw FailWriteException();

                if(setjmp(png_jmpbuf(png))) throw FailWriteException();
                png_init_io(png, fp);

                png_set_IHDR(
                        png,
                        info,
                        m_img.Width(),
                        m_img.Height(),
                        8,// 8 bit RGBA
                        PNG_COLOR_TYPE_RGBA,
                        PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT,
                        PNG_FILTER_TYPE_DEFAULT);
                png_write_info(png, info);
                // if(no alpha)
                //  png_set_filler(png, 0, PNG_FILLER_AFTER);
                auto data = m_img.Data();
                auto data_pointer = data.data();

                png_write_image(png, reinterpret_cast<png_bytepp>(data_pointer));
                png_write_end(png, NULL);

                png_destroy_write_struct(&png, &info);
                fclose(fp);
            }
            catch(FailWriteException&)
            {
                if(fp) { fclose(fp); }
                else { throw FailWriteException(std::string("File unavailable for writing (cannot access)")); }
                if(png)
                    png_destroy_write_struct(&png, NULL);
                else { throw FailWriteException(std::string("Could not create PNG file")); }
                if(info)
                    png_destroy_write_struct(NULL, &info);
                else { throw FailWriteException(std::string("Could not create PNG headers")); }
            }
        }
        else if(out_path.extension() == ".ppm")
        {
            std::ofstream out_file(out_path, std::ios::binary);
            if(!out_file.is_open()) { return false; }
            out_file << "P6\n"
                     << m_img.Width() << " " << m_img.Height() << "\n255\n";
            for(size_t i = 0; i < m_img.Width(); ++i)
            {
                for(size_t j = 0; j < m_img.Height(); ++j)
                {
                    const Pixel &p = m_img.Get(i, j);
                    out_file << p.r << p.g << p.b;
                }
            }
            out_file.close();
        }
        else
        {
            std::cerr << "File type: " << out_path.extension()
                      << " is not supported" << std::endl;
        }
        return true;
    }
    Image ImageIO::read(const std::filesystem::path& in_path)
    {
        if(in_path.extension() == ".ppm")
        {
            std::ifstream in_file(in_path, std::ios::binary);
            if(!in_file.is_open()) { throw std::runtime_error("File could not be found"); }
            std::string line;

            in_file >> line;// Uselss
            throw std::runtime_error("Filetype not supported");
        }
        else if(in_path.extension() == ".png")
        {
            if(!std::filesystem::exists(in_path))
            {
                throw std::runtime_error("No such file found: "
                                         + in_path.string());
            }
            // https://fossies.org/linux/libpng/example.c
            png_image png_image;
            memset(&png_image, 0, sizeof(png_image));
            png_image.version = PNG_IMAGE_VERSION;

            if(png_image_begin_read_from_file(&png_image, in_path.c_str())
               != 0)
            {
                png_image.format = PNG_FORMAT_RGBA;
                png_bytep buffer = static_cast<unsigned char*>(
                        malloc(PNG_IMAGE_SIZE(png_image)));
                if(buffer != NULL
                   && png_image_finish_read(&png_image, NULL, buffer, 0, NULL)
                      != 0)
                {
                    // Convert png image to pixels
                    std::vector<Pixel> pixels{};
                    for(size_t i = 0; i < PNG_IMAGE_SIZE(png_image); i += 4)
                    {
                        pixels.emplace_back(buffer[i],
                                            buffer[i + 1],
                                            buffer[i + 2],
                                            static_cast<double>(buffer[i + 3])
                                            / 255.);
                    }
                    Image image(png_image.width, png_image.height, pixels);
                    return image;
                }
                else
                {
                    if(buffer == NULL) { png_image_free(&png_image); }
                    else
                    {
                        free(buffer);
                    }
                    throw std::runtime_error("Could not allocate image");
                }
            }
            else
            {
                throw std::runtime_error("Could not read PNG file");
            }
        }
        else
        {
            ruff::logError("Filetype not supported");
            throw std::runtime_error("Filetype not supported");
        }
    }
}
