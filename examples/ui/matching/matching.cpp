// STL Packages
#include <memory>

// Source
#include "ui/ui.hpp"
#include "match.hpp"

using sint = short int;

class Matching : public ruff::ui::Engine
{
 private:
	std::vector<ruff::Point2D<sint>> drawing{};
	ruff::match::Matcher             matcher{};


 public:
	Matching(const sint width, const sint height, std::string title = "Matching Engine", int pixelRatio = 1)
	  : Engine(height, width, title, pixelRatio)
	{
		matcher = ruff::match::Matcher(ruff::match::DetectorType::ORB, ruff::match::MatcherType::BF);
		static_cast<cv::ORB*>(matcher.getDetector().get())->setPatchSize(250);
	}

	Matching(const Matching& other) = delete;

	Matching& operator=(const Matching& other) = delete;


	virtual void onCreate() override
	{
	}
	virtual void onUpdate(double deltaTime) override
	{
		(void)deltaTime;

		if(mouse.mouse_pressed[0])
		{
			drawing.emplace_back(mouse.mouse_x, mouse.mouse_y);
		}


		clearScreen();


		// Draw the drawing regions
		drawSquare(5, 5, getWidth() / 2 - 5, getHeight() - 5, ruff::ui::WHITE, false);
		drawSquare(getWidth() / 2 + 5, 5, getWidth() - 5, getHeight() - 5, ruff::ui::WHITE, false);

		bool flip = false;
		for(const auto& point : drawing)
		{
			if(flip)
				drawCircle(point, 5, ruff::ui::GREEN, true);
			else
				drawSquare(point.x - 5, point.y - 5, point.x + 5, point.y + 5, ruff::ui::GREEN, true);
			flip = !flip;
		}

		if(drawing.size() > 0)
		{
			// Convert regions to cv::Mats
			auto left  = getCVMat(5, 5, getWidth() / 2 - 5, getHeight() - 5);
			auto right = getCVMat(getWidth() / 2 + 5, 5, getWidth() - 5, getHeight() - 5);

			// Convert to gray
			cvtColor(left, left, cv::COLOR_RGB2GRAY);
			cvtColor(right, right, cv::COLOR_RGB2GRAY);

			// Match
			auto result = matcher.match(left, right);

			const std::vector<cv::KeyPoint>& kptsL = std::get<1>(result);
			const std::vector<cv::KeyPoint>& kptsR = std::get<2>(result);

			// Draw matches on drawing window
			for(const auto match : std::get<0>(result))
			{
				const int LIdx = match.queryIdx;
				const int RIdx = match.trainIdx;
				drawLine(kptsL[LIdx].pt.x + 5, kptsL[LIdx].pt.y + 5, kptsR[RIdx].pt.x + getWidth() / 2 + 5, kptsR[RIdx].pt.y + 5, ruff::ui::getRandColor());
			}
		}
	}
};

int main()
{
	// Create game engine and then run it
	Matching matchingEngine(1000, 1500, "Matching", 2);
	matchingEngine.launch();
	return 0;
}
