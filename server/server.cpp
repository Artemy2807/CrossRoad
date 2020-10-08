#include "server.h"

Server::Server() {
	isconnected = false;
	portno = 1111; 
	start();
}

void sockOptEnable(int sockfd, int optName)
{
	int32_t optval = 1;
	size_t optlen = sizeof(optval);
	if (setsockopt(sockfd, SOL_SOCKET, optName, (char *)&optval, optlen) < 0)
	{
		perror("sockOptEnable()");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

void Server::start() {
	if (isconnected) return;
	LOG("Server started");
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		LOG("ERROR opening socket");
	}

	sockOptEnable(sockfd, SO_KEEPALIVE);
	sockOptEnable(sockfd, SO_REUSEADDR);

	bzero((char *)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	while (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		LOG("ERROR binding socket");
		sleep(1); //wait 1 sec befor next try
	}

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	LOG("waiting for client");
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	LOG("client connected");
	isconnected = true;

	uint16_t width = (uint16_t)sys->capture_width;
	uint16_t height = (uint16_t)sys->capture_height;
	send_data(&width, newsockfd, 2);
	send_data(&height, newsockfd, 2);

	return;
}

void Server::stop() {
	close(newsockfd);
	close(sockfd);
}

void Server::send(dataType type, uint32_t dataSize, void *ptr)
{
	bool status = true;
	uint32_t tp = type;
	if (!send_data((void *)&tp, newsockfd, sizeof(uint32_t)))
	{
		status = false;
	}

	if (!send_data((void *)&dataSize, newsockfd, sizeof(uint32_t)))
	{
		status = false;
	}
	if (!send_data(ptr, newsockfd, (size_t)dataSize))
	{
		status = false;
	}


	if (status)
	{
		return;
	}
	else
	{
		LOG("[I]: Client disconnected");
		isconnected = false;
		stop();
		start();
		return;
	}
}

void server_fnc(void *ptr) {
	Engine eng;
	Recognition &rec = *((*Recognition)ptr);
	std::vector<int> parameters; //вектор параметров сжатия изображения(jpeg 20%)
	std::vector<cv::uchar> buffer; //вектор содержащий сжатое изображение
	parameters = std::vector<int>(2);
	parameters[0] = cv::CV_IMWRITE_JPEG_QUALITY; //jpeg
	parameters[1] = rec.qualityImage; //0-100 quality
	Server ser;
	while (true) {
		rec.getEngine(eng);
		Mat &frame = *(rec.image)
		imencode(".jpg", frame, buffer, parameters);
		ser.send(Image_t, (uint32_t)buffer.size(), (void *)(&buffer[0]));
		ser.send(Engine_t, sizeof(Engine), (void *)(&eng));
	}
}