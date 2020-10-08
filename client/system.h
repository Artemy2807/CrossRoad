#ifndef SYSTEM_H
#define SYSTEM_H

#include <opencv2/opencv.hpp>
#include "Engine.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

class System {
public:
	cv::Mat image;
	Engine eng;
	uint32_t portno;
	char* host;
	System();
	void setImage(cv::Mat& obj);
	cv::Mat getImage();
	void setEngine(Engine& obj);
};

#endif