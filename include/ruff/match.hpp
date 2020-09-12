//STD
#include <vector>
#include <memory>
#include <cstdint>

//Packages
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "logger.hpp"

namespace ruff
{
	namespace match
	{
		using kptMatch = std::tuple<std::vector<cv::DMatch>, std::vector<cv::KeyPoint>, std::vector<cv::KeyPoint>>;

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Types of opencv detectors available 
		 */
		/* ----------------------------------------------------------------------------*/
		enum class DetectorType : uint_fast8_t
		{
			ORB,
			AKAZE,
			BRISK,
			DAISY
		};
		static constexpr DetectorType DetectorTypes[] = {
			DetectorType::ORB, DetectorType::AKAZE, DetectorType::BRISK, DetectorType::DAISY};

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Types of matchers offered by opencv
		 */
		/* ----------------------------------------------------------------------------*/
		enum class MatcherType : uint_fast8_t
		{
			BF,
			FLANN
		};
		static constexpr MatcherType MatcherTypes[] = {MatcherType::BF , MatcherType::FLANN};

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Class that uses OpenCV for feature matching
		 */
		/* ----------------------------------------------------------------------------*/
		class Matcher 
		{
		private:
			DetectorType dType{};
			MatcherType mType{};
			cv::Ptr<cv::Feature2D> detector{};
			std::unique_ptr<cv::DescriptorMatcher> matcher{nullptr};
			bool knnMatch{false};


		public:
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis Create a OpenCV detector for feature matching
			 *
			 * @Param detectorType OpenCV Keypoints finder such as ORB or AKAZE 
			 * @Param matcherType OpenCV Keypoint matcher such as BF or FLANN
			 * @Param knnMatch Whether or not the algoithm will use knnMatching or match
			 */
			/* ----------------------------------------------------------------------------*/
			Matcher(DetectorType detectorType, MatcherType matcherType, bool knnMatch = false);

			Matcher() = default;

			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis Takes two images and find the matching features between the two images
			 *
			 * @Param reference Reference image
			 * @Param target Target image
			 *
			 * @Returns All good matches and keypoints found in both images as a tuple
			 * {good_matches, kpts1, kpts2}
			 */
			/* ----------------------------------------------------------------------------*/
			[[nodiscard]] kptMatch match(const cv::Mat& reference, const cv::Mat& target) const;

			[[nodiscard]] cv::Ptr<cv::Feature2D> getDetector() const { return detector; };
		};

	};
}
