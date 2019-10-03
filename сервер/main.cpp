#include "recognitionVehicle.h"
#include "Trafficlight.h"
#include <pthread.h> 
#include "server.h"


int main(int argc, char* argv[]) {
	Recognition rec;

	pthread_t recognize_vehicle_thr;
	pthread_create(&recognize_vehicle_thr, NULL, rec.recognition);

	pthread_t main_trafficLight_thr;
	pthread_create(&main_trafficLight_thr, NULL, main_trafficLight_fnc, &rec);

	pthread_t secondary_trafficLight_thr;
	pthread_create(&secondary_trafficLight_thr, NULL, second_trafficLight_fnc, &rec);

	pthread_t server_thr;
	pthread_create(&server_thr, NULL, server_fnc, &rec);
}