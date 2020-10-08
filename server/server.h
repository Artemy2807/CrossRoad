#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "recognitionVehicle.h"

#define LOG(msg); printf("%s\n", msg);

enum dataType
{
	Image_t = 0,
	Engine_t = 1
};

class Server {
private:
	bool isconnected = false;
	int portno; 
	int sockfd;
	int newsockfd;
	void start();
public:
	Server();
	void stop();
	void send();
};

#endif
