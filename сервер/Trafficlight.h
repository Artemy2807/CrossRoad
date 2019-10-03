#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "recognitionVehicle.h"
#include "TrafficLightPins.h"
#include <stdio.h>
#include <vector>

#define RETURN_FLASHING_GREEN 2;
#define YELLOW_TIME 2
#define SECOND 1000

class MainTrafficLight {
private:
	int greenTime = 5, redTime = 15;
	int posledMainTLight = 0;
public:
	MainTrafficLight();
	void start(const Recognition& ptr);
};

class SecondTrafficLight {
private:
	int greenTime = 15, redTime = 5;
	int posledSecondaryTLight = 0;
public:
	SecondTrafficLight();
	void start(const Recognition& ptr);
};

#endif