#include "Trafficlight.h"

MainTrafficLight::MainTrafficLight()
{
	pinMode(RED_PINS_M, OUTPUT);
	pinMode(YELLOW_PINS_M, OUTPUT);
	pinMode(GREEN_PINS_M, OUTPUT);
	pinMode(DOPSECTION_PINS_M, OUTPUT);
}

SecondTrafficLight::SecondTrafficLight() {
	pinMode(RED_PINS_S, OUTPUT);
	pinMode(YELLOW_PINS_S, OUTPUT);
	pinMode(GREEN_PINS_S, OUTPUT);
}

void MainTrafficLight::start(const Recognition& ptr) {
	if (redTime == 15 && greenTime == 5) digitalWrite(DOPSECTION_PINS_M, HIGH);

	switch (posledMainTLight) {
	 case 0:
		//¬ключает красный сигнал на заданное врем€, а потом выключает
		if (millis() - time <= SECOND * redTime) digitalWrite(RED_PINS_M, HIGH);
		else {
			digitalWrite(RED_PINS_M, LOW);
			time = millis();
			posledMainTLight = 1;
		}
	 break;
	 case 1:
		//¬ключает жЄлтый сигнал на заданное врем€, а потом выключает
		if (millis() - time <= SECOND * YELLOW_TIME) digitalWrite(YELLOW_PINS_M, HIGH);
		else {
			digitalWrite(YELLOW_PINS_M, LOW);
			time = millis();
			posledMainTLight = 2;
		}
	 break;
	 case 2:
		/*
			¬ключает зелЄный сигнал на заданное врем€, далее мигает заданное
			кол-во раз и выключает зелЄный сигнал
		  */
		if (millis() - time <= SECOND * (greenTime - RETURN_FLASHING_GREEN)) digitalWrite(GREEN_PINS_M, HIGH);
		else {
			digitalWrite(GREEN_PINS_M, LOW);
			time = millis();
			for (int i = 0; i < RETURN_FLASHING_GREEN; i++) {
				if (millis() - time <= (RETURN_FLASHING_GREEN * SECOND) / RETURN_FLASHING_GREEN) digitalWrite(GREEN_PINS_M, HIGH);
				else {
					digitalWrite(GREEN_PINS_M, LOW);
					time = millis();
				}
			}
			time = millis();
			posledMainTLight = 3;
		}
	 break;
	 case 3:
		if (millis() - time <= SECOND * YELLOW_TIME) digitalWrite(YELLOW_PINS_M, HIGH);
		else {
			digitalWrite(YELLOW_PINS_M, LOW);
			time = millis();
			posledMainTLight = 0;
			this->redTime = ptr.redTime;
			this->greenTime = ptr.greenTime;
		}
	 break;
	}
}

void SecondTrafficLight::start(const Recognition& ptr) {
	switch (posledSecondaryTLight) {
	 case 0:
	  /*
		 ¬ключает зелЄный сигнал на заданное врем€, далее мигает заданное
		 кол-во раз и выключает зелЄный сигнал
		*/
		if (millis() - timeSecond <= SECOND * (redTime - 2)) digitalWrite(GREEN_PINS_S, HIGH);
		else {
			digitalWrite(GREEN_PINS_S, LOW);
			timeSecond = millis();
			for (int i = 0; i < RETURN_FLASHING_GREEN; i++) {
				if (millis() - time <= (RETURN_FLASHING_GREEN * SECOND) / RETURN_FLASHING_GREEN) digitalWrite(GREEN_PINS_S, HIGH);
				else {
					digitalWrite(GREEN_PINS_S, LOW);
					timeSecond = millis();
				}
			}
			timeSecond = millis();
			posledSecondaryTLight = 1;
		}
	 break;
	 case 1:
	    //¬ключает жЄлтый сигнал на заданное врем€, а потом выключает
		if (millis() - timeSecond <= SECOND * YELLOW_TIME) digitalWrite(YELLOW_PINS_S, HIGH);
		else {
			digitalWrite(YELLOW_PINS_S, LOW);
			timeSecond = millis();
			posledSecondaryTLight = 2;
		}
	 break;
	 case 2:
		//¬ключает красный сигнал на заданное врем€, а потом выключает
		if (millis() - timeSecond <= SECOND * greenTime) digitalWrite(RED_PINS_S, HIGH);
		else {
			digitalWrite(RED_PINS_S, LOW)
			timeSecond = millis();
			posledSecondaryTLight = 3;
		}
	 break;
	 case 3:
		//¬ключает жЄлтый сигнал на заданное врем€, а потом выключает
		if (millis() - timeSecond <= SECOND * YELLOW_TIME) digitalWrite(YELLOW_PINS_S, HIGH);
		else {
			digitalWrite(YELLOW_PINS_S, LOW);
			timeSecond = millis();
			posledSecondaryTLight = 0;
			this->redTime = ptr.redTime;
			this->greenTime = ptr.greenTime;
		}
	 break;
	}
}

void main_trafficLight_fnc(void *ptr) {
	Recognition &rec = *((*Recognition)ptr);
	MainTrafficLight Mtr;
	while (true) {
		Mtr.start(rec);
	}
}

void second_trafficLight_fnc(void *ptr) {
	Recognition &rec = *((*Recognition)ptr);
	SecondTrafficLight Str;
	while (true) {
		Str.start(rec);
	}
}