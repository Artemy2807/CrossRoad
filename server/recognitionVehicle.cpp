#include "recognitionVehicle.h"

Recognition::Recognition(const int& qualityImage) {
	Ptr<SVM> svm;
	hog.winSize = SIZE_TRANNING_SVM;
	svm = StatModel::load<SVM>(SVM_MODEL);
	std::vector<float> descriptors;
	get_svm_detector(svm, descriptors);
	hog.setSVMDetector(descriptors);
	this->qualityImage = qualityImage;
	lastTime = clock();
}

void Recognition::recognition() {
	cv::VideoCapture vid(VIDEO);

	if (!capture.isOpened()) {
		printf("[E]: wrong path to video file or video file is damaged\n");
		exit(1);
	}
	cv::Mat frame;
	while (true) {
		eng.centroids.clear();
		eng.locVehicles.clear();
		vid >> frame;
		if (frame.empty()) {
			printf("[E]: frame in the video is damaged\n");
			exit(0);
		}
		hog.detectMultiScale(frame, eng.locVehicles, 0, Size(8, 8), Size(0, 0), 0.9, 2);
		centroids = getCentroid(eng.locVehicles);
		nowTime = clock();
		if (nowTime - lastTime >= SECOND * 12) {
			if (eng.numofVehicles > 4) {
				this->redTime = eng.numofVehicles * 1.2
				redTime = redTime>15 ? 15 : (redTime<3 ? 3 : redTime);
				this->greenTime = 20 - redTime;
			}
			else {
				this->redTime = 15;
				this->greenTime = 5;
			}
			lastTime = clock();
		}
	}
}

std::vector<cv::Point> Recognition::getCentroid(const std::vector<cv::Rect>& locations) {
	std::vector<cv::Point> centroids;
	for (const cv::Rect& loc : locations) {
		cv::Point cent;
		cent.x = (int)loc.x + (loc.width - loc.x) / 2;
		cent.y = (int)loc.y + (loc.height - loc.y) / 2;
		eng.centroids.push_back(cent);
		centroids.push_back(cent);
	}
	return centroids;
}

void Recognition::get_svm_detector(const Ptr<SVM>& svm, std::vector<float>& hog_detector)
{

	cv::Mat sv = svm->getSupportVectors();
	const int sv_total = sv.rows;

	cv::Mat alpha, svidx;
	double rho = svm->getDecisionFunction(0, alpha, svidx);

	CV_Assert(alpha.total() == 1 && svidx.total() == 1 && sv_total == 1);
	CV_Assert((alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
		(alpha.type() == CV_32F && alpha.at<float>(0) == 1.f));
	CV_Assert(sv.type() == CV_32F);
	hog_detector.clear();
	hog_detector.resize(sv.cols + 1);
	memcpy(&hog_detector[0], sv.ptr(), sv.cols * sizeof(hog_detector[0]));
	hog_detector[sv.cols] = (float)-rho;
}

void Recognition::updateVehicles() {

	for (int i = 0; i < Vehicles.size(); i++) {
		int value = vehiclesisNew(i);
		if (value != -1) {
			centroids.erase(centroids.begin() + value);
		}
	}

	for (const cv::Point& centroid : centroids) {
		vehicle newVehicle;
		newVehicle.index = Vehicles.size() + 1;
		newVehicle.wayVehicle.push_back(centroid);
		Vehicles.push_back(newVehicle);
	}

	for (int i = 0; i < Vehicles.size(); i++) {

		int lastindex = Vehicles[i].wayVehicle.size() - 1;
		if (!Vehicles[i].countVehicle && Vehicles[i].wayVehicle[lastindex] > 130) {
			eng.numofVehicles++;
			Vehicles[i].countVehicle = true;
		}

		if (Vehicles[i].framesSinceSeen > MAX_UNSEEN_FRAMES) Vehicles[i].clear();
	}
}

int Recognition::vehiclesisNew(const int& index) {
	int number = 0;
	for (const cv::Point& centroid : centroids) {
		int lastindex = Vehicles[i].wayVehicle.size() - 1;
		std::vector<float> distAndAngle = getAngleAndDist(Vehicles[i].wayVehicle[lastindex], centroid);
		if (isValidVector(distAndAngle)) {
			Vehicles[i].wayVehicle.push_back(centroid);
			return number;
		}
		number++;
	}
	Vehicles[i].framesSinceSeen++;
	return -1;
}

/*
  ‘ункци€ возвращ€ет рассто€ние и угол от точки a до точки b.
  ”гол может быть в районе от 180 до -180.
*/

std::vector<float> Recognition::getAngleAndDist(const cv::Point& a, const cv::Point& b) {

	std::vector<float> distAndAngle;

	float dx = (b.x - a.x);
	float dy = (b.y - a.y);
	float angle = 0;
	float distance = sqrt(dx * dx + dy * dy);

	distAndAngle.push_back(distance);

	if (dy > 0)
	{
		angle = atan(-dx / dy); angle = angle * 180 / M_PI;
	}
	else if (dy == 0) {
		if (dx < 0) angle = 90.0;
		else if (dx > 0) angle = -90.0;
		else angle = 0.0;
	}
	else {
		if (dx < 0) {
			angle = atan(dx / dy);
			angle = angle * 180 / M_PI;
			angle = 180 - angle;
		}
		else if (dx > 0) {
			angle = atan(dx / dy);
			angle = angle * 180 / M_PI;
			angle = (-180) - angle;
		}
		else angle = 180.0;
	}
	distAndAngle.push_back(angle);
	return distAndAngle;
}

bool Recognition::isValidVector(std::vector<float> a) {
	float distance = a[0];
	float angle = a[1];
	float treshold_distance = max(120.0, -0.008 * angle * angle + 0.4 * angle + 25.0);
	return (distance <= treshold_distance);
}

void Recognition::getEngine(Engine& engine) {
	engine = eng;
}