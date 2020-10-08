#ifndef RECOGHITION_VEHICLE_H
#define RECOGHITION_VEHICLE_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <stdio.h>
#include <vector>
#include <ctime>

#define SVM_MODEL "../HOG/mySVM.yml"
#define VIDEO "../Video/trassa.mp4"
#define SIZE_TRANNING_SVM cv::Size(64, 64)
#define M_PI  3.14159265358979323846
#define MAX_UNSEEN_FRAMES 20
#define SECOND 1000000

struct Engine {
	int numofVehicles = 0;
	std::vector<cv::Point> centroids;
	std::vector<cv::Rect> locVehicles;
};

struct vehicle {
	int index;
	int framesSinceSeen = 0;
	bool countVehicle = false;
	std::vector<cv::Point> wayVehicle;
};

class Recognition {
private:
	HOGDescriptor hog;
	Engine eng;
	std::vector<cv::Point> centroids;
	std::vector<vehicle> Vehicles;
	clock_t nowTime, lastTime;
	void get_svm_detector(const Ptr<SVM>& svm, std::vector<float>& hog_detector);
	std::vector<cv::Point> getCentroid(const std::vector<cv::Rect>& locations);
	void updateVehicles();
	std::vector<float> getAngleAndDist(const cv::Point& a, const cv::Point& b);
	int vehiclesisNew(const int& index);
	bool isValidVector(std::vector<float> a);
public:
	int qualityImage;
	int redTime = 0, greenTime = 0;
	cv::Mat image;
	Recognition();
	void recognition();
	void getEngine(Engine& engine);
};

#endif