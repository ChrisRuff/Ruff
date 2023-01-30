#include <ruff/photogrammetry/models/camera.hpp>

#include <ruff/photogrammetry/utility/conversions.hpp>
namespace ruff::photogrammetry
{
imgproc::Image Camera::Undistort(const imgproc::Image& img) const
{
		cv::Mat cv_img = convert(img);
		auto intrin = intrinsics.Mat();
		auto distCoeffs = coeffs.Mat();

		cv::undistort(cv_img, cv_img, intrin, distCoeffs);

		return convert(cv_img);
}

void Camera::Pack(flatbuffers::FlatBufferBuilder& builder) const
{
		IntrinsicsBuf intrin_buf(intrinsics.fx, intrinsics.fy, intrinsics.cx, intrinsics.cy);
		ExtrinsicsBuf extrin_buf(flatbuffers::span<const double, 9>(extrinsics.rotation.data(), 9),
		                         flatbuffers::span<const double, 3>(extrinsics.translation.data(), 3));
		DistortionCoeffsBuf coeff_buf(flatbuffers::span<const double, 3>(coeffs.radial.data(), 3),
		                         flatbuffers::span<const double, 2>(coeffs.tangential.data(), 2));
		auto offset = CreateCameraBuf(builder, &intrin_buf, &extrin_buf, &coeff_buf);
		builder.Finish(offset);
}
Camera Camera::Unpack(const void* buffer)
{
		const CameraBuf* cam_buf = GetCameraBuf(buffer);
		Camera cam;
		cam.intrinsics.fx = cam_buf->intrinsics()->fx();
		cam.intrinsics.fy = cam_buf->intrinsics()->fy();
		cam.intrinsics.cx = cam_buf->intrinsics()->cx();
		cam.intrinsics.cy = cam_buf->intrinsics()->cy();

		std::copy(cam_buf->extrinsics()->rotation()->begin(),
		          cam_buf->extrinsics()->rotation()->end(),
		          cam.extrinsics.rotation.begin());
		std::copy(cam_buf->extrinsics()->translation()->begin(),
							cam_buf->extrinsics()->translation()->end(),
							cam.extrinsics.translation.begin());

		std::copy(cam_buf->dist_coeffs()->radial()->begin(),
							cam_buf->dist_coeffs()->radial()->end(),
							cam.coeffs.radial.begin());
		std::copy(cam_buf->dist_coeffs()->tangential()->begin(),
							cam_buf->dist_coeffs()->tangential()->end(),
							cam.coeffs.tangential.begin());
		return cam;
}
}
