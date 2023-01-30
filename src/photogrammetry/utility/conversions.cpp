#include <ruff/photogrammetry/utility/conversions.hpp>

namespace ruff
{
		template<typename T>
		cv::Mat convert(core::Tensor<T>& tensor)
		{
		    return cv::Mat_<T>(tensor.rows(), tensor.cols(), tensor.GetData().data);
		}
		template<typename T>
		cv::Mat convert(const core::Tensor<T>& tensor)
		{
				return cv::Mat_<T>(tensor.rows(), tensor.cols(), tensor.GetData().data).clone();
		}

		cv::Mat convert(const imgproc::Image& img)
		{
		    auto data = img.BGRData();
				return cv::Mat(img.Height(), img.Width(), CV_8UC4,
		                   data.data(), cv::Mat::AUTO_STEP).clone();
		}
    imgproc::Image convert(cv::Mat cv_img)
    {
		    imgproc::Image img(cv_img.cols, cv_img.rows);
		    uint8_t* cv_mat_ptr = static_cast<uint8_t*>(cv_img.data);

		    if(cv_img.channels() == 3)
		    {
						for(size_t i = 0; i < img.Height(); ++i)
						{
								for(size_t j = 0; j < img.Width(); ++j)
								{
										img.Set(i, j,
								            imgproc::Pixel(
								              cv_mat_ptr[i * cv_img.cols * 3 + j*3 + 0],
															cv_mat_ptr[i * cv_img.cols * 3 + j*3 + 1],
															cv_mat_ptr[i * cv_img.cols * 3 + j*3 + 2]));
								}
						}
		    }
		    else if(cv_img.channels() == 1)
		    {
						cv_img.convertTo(cv_img, CV_8UC1);
						for(size_t i = 0; i < img.Height(); ++i)
						{
								for(size_t j = 0; j < img.Width(); ++j)
								{
										img.Set(i, j, cv_img.at<uchar>(i, j));
								}
						}
		    }
				return img;
    }

    }
