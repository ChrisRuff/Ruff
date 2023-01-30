#pragma once
#include <array>

#include <ruff/core/models/tensor.hpp>
#include <ruff/imgproc/image.hpp>

#include <ruff/photogrammetry/buffer/camera_buf.hpp>

namespace ruff::photogrammetry
{
		struct Intrinsics
		{
				double fx, fy;
				double cx, cy;

				std::array<double, 9> Mat() const
				{
						return {fx, 0, cx,
										0, fy, cy,
										0, 0, 1 };
				}
		};
		struct Extrinsics
		{
				std::array<double, 9> rotation;
				std::array<double, 3> translation;
				std::array<double, 12> Mat() const
				{
						return {rotation[0], rotation[1], rotation[2], translation[0],
										rotation[3], rotation[4], rotation[5], translation[1],
										rotation[6], rotation[7], rotation[8], translation[2] };
				}
		};
		struct DistortionCoeffs
		{
				std::array<double, 3> radial;
				std::array<double, 2> tangential;

				std::array<double, 5> Mat() const
				{
						return {radial[0], radial[1], radial[2], tangential[0], tangential[1]};
				}
		};
		class Camera
		{
				Intrinsics intrinsics;
				Extrinsics extrinsics;
				DistortionCoeffs coeffs;
		  public:
		    imgproc::Image Undistort(const imgproc::Image& img) const;

				void Pack(flatbuffers::FlatBufferBuilder&) const;
				Camera static Unpack(const void*);
		};
    };