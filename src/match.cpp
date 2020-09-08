#include "match.hpp"
//#include <opencv2/xfeatures2d.hpp>

namespace ruff
{
	namespace match
	{
		Matcher::Matcher(DetectorType detectorType, MatcherType matcherType, 
				bool knnMatch) : dType(detectorType), mType(matcherType), knnMatch(knnMatch)
		{
			// Set detector
			if(detectorType == DetectorType::AKAZE)
			{
				this->detector = cv::AKAZE::create();
			}
			else if(detectorType == DetectorType::BRISK)
			{
				this->detector = cv::BRISK::create();
			}
			//else if(detectorType == DetectorType::DAISY)
			//{
			//	assert(("Daisy is only supported with FLANN matcher", matcherType == MatcherType::FLANN));
			//	this->detector = cv::xfeatures2d::DAISY::create();
			//}
			else if(detectorType == DetectorType::ORB)
			{
				this->detector = cv::ORB::create();
			}

			// Set Matcher
			if(matcherType == MatcherType::BF)
			{
				if(knnMatch)
				{
					this->matcher = std::make_unique<cv::BFMatcher>(cv::NORM_HAMMING, false);
				}
				else
				{
					this->matcher = std::make_unique<cv::BFMatcher>(cv::NORM_HAMMING, true);
				}
			}
			else
			{
				this->matcher = std::make_unique<cv::FlannBasedMatcher>();
			}
		}

		kptMatch Matcher::match(const cv::Mat& reference, const cv::Mat& target)
		{
			std::vector<cv::KeyPoint> kpts1, kpts2;
			cv::Mat desc1, desc2;

			//if(dType == DetectorType::DAISY)
			//{
			//	int diameter = 25;
			//	// Add every pixel to the list of keypoints for each image
			//	for (double xx = diameter; xx < reference.size().width - diameter; ++xx) 
			//	{
			//		for (double yy = diameter; yy < reference.size().height - diameter; ++yy) 
			//		{
			//			kpts1.push_back(cv::KeyPoint(xx, yy, diameter));
			//		}
			//	}
			//	for (double xx = diameter; xx < target.size().width - diameter; ++xx) 
			//	{
			//		for (double yy = diameter; yy < target.size().height - diameter; ++yy) 
			//		{
			//			kpts2.push_back(cv::KeyPoint(xx, yy, diameter));
			//		}
			//	}
			//	detector->compute(reference, kpts1, desc1);
			//	detector->compute(target, kpts2, desc2);
			//}
			//else
			{
				// Find features on both images
				detector->detect(reference, kpts1);
				detector->compute(reference, kpts1, desc1);
				detector->detect(target, kpts2);
				detector->compute(target, kpts2, desc2);
			}


			if(mType == MatcherType::FLANN)
			{
				if(desc1.type()!=CV_32F) desc1.convertTo(desc1, CV_32F);
				if(desc2.type()!=CV_32F) desc2.convertTo(desc2, CV_32F);
			}

			if(desc1.empty() || desc2.empty())
			{
				logWarning("Couldn't find any matches");
				return kptMatch{};
			}
			std::vector<cv::DMatch> good_matches;
			if(knnMatch)
			{
				std::vector<std::vector<cv::DMatch>> matches;
				matcher->knnMatch(desc1, desc2, matches, 2);
				const size_t size = matches.size();
				good_matches.reserve(size);
				for (size_t i = 0; i < size; ++i)
				{
					if (matches[i][0].distance < .75 * matches[i][1].distance)
					{
						good_matches.push_back(matches[i][0]);
					}
				}
			}
			else
			{
				std::vector<cv::DMatch> matches;
				matcher->match(desc1, desc2, matches);

				const size_t size = matches.size();
				good_matches.reserve(size);

				// Calculate minimum and max distance between matches
				double max_dist = 0; double min_dist = 100;
				for (size_t i = 0; i < size; ++i)
				{
					double dist = matches[i].distance;
					min_dist = MIN(min_dist, dist);
					max_dist = MAX(max_dist, dist);
				}
				for (size_t i = 0; i < size; ++i)
				{
					// Extract good matches from all matches
					if (matches[i].distance <= MAX(6 * min_dist, 0.02))
					{
						good_matches.push_back(matches[i]);
					}
				}
			}
			return std::make_tuple(good_matches, kpts1, kpts2);
		}
	};
};
