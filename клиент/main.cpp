#include <opencv2/opencv.hpp>
#include "Engine.h"
#include "client.h"
#include "system.h"
#include <thread>

#define Width 320
#define Height 640
#define BorderSize 100
Client *client;
System syst;
cv::Mat window; //Главное окно
cv::Mat wroi; //Область на главном окне, где будет находиться изображение
cv::Mat panel; //Область на изображение где будет выводиться информация

int main(int argc, char** argv) {
	client = new Client(syst);

	thread thr(client_fnc, ref(syst), ref(*client));
	thr.detach();

	printf("[I]: Connecting to %s:%d...\n", syst.host, syst.portno);

	if (!client->connect())
	{
		printf("[E]: Connection failed.\n");
		error("Can't connect to server.");
	}
	printf("Connection was successfully established!\n");

	window = cv::Mat(Height, Width + BorderSize, CV_8UC3, cv::Scalar(255, 255, 255)); //создаём главное окно
	wroi = window(cv::Rect(cv::Point(0, 0), cv::Point(width, height)));
	panel = window(cv::Rect(cv::Point(width - 1, 0), cv::Point(width + bordersize - 1, height - 1)));

	cv::namedWindow("Stream", cv::WINDOW_NORMAL | CV_WINDOW_KEEPRATIO /*| WINDOW_OPENGL*/); //создаём главное окно

	while (true) {
		wroi = syst.getImage();
		cv::imshow("Stream", window);
		if (cv::waitKey(1) == 27)break;
	}
}