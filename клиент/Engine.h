#ifndef ENGINE_H
#define ENGINE_H

#include <opencv2/opencv.hpp>
#include <vector>

struct Engine {
	int numofVehicles = 0;
	std::vector<cv::Point> centroids;
	std::vector<cv::Rect> locVehicles;
};

#endif