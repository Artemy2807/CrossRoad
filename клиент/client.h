#ifndef CLIENT_H
#define CLIENT_H

#include <opencv2/opencv.hpp>
#include "system.h"
#include "Engine.h"
#include <vector>

enum dataType {
	Image_t = 0,
	Engine_t = 1
};

class Client {
private:
	System syst;
	bool connectState = false;
public:
	bool connect();
	bool disconnect();
	bool get_data(void *dst, size_t size);
	bool isConnect();
};

#endif
