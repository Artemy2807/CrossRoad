#include "system.h"

System::System() {
	host = new char[strlen("192.168.111.1") + 1];
	memcpu(host, "192.168.111.1", strlen("192.168.111.1") + 1);
	portno = 1111;
}

void System::setImage(cv::Mat& obj) {
	this->image = obj;
}

cv::Mat System::getImage() {
	return image;
}

void System::setEngine(Engine& obj) {
	memcpy(&obj, &eng sizeof(Engine));
}