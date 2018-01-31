//
//
//
//
//


#include <ros/ros.h>
#include <uav_abstraction_layer/ual.h>
#include <iostream>
#include <string>
#include <argument_parser/argument_parser.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include <simple_sockets_cpp/com/Socket.h>

BOViL::comm::Socket* gSocket;
cv::Mat up, down, left, right, w, s, t, l, bg;


int main(int _argc, char** _argv) {
	if(_argc < 3){
		std::cout << "Provide base path to images and ip"<<std::endl;
		return 0;
	}
	std::string basePath = _argv[1];
	up = cv::imread(basePath+"up.png",-1);
	down = cv::imread(basePath+"down.png",-1);
	left = cv::imread(basePath+"left.png",-1);
	right = cv::imread(basePath+"right.png",-1);
	w = cv::imread(basePath+"w.png",-1);
	s = cv::imread(basePath+"s.png",-1);
	t = cv::imread(basePath+"t.png",-1);
	l = cv::imread(basePath+"l.png",-1);
	bg = cv::imread(basePath+"bg.png",-1);

	cv::namedWindow("display", CV_WINDOW_AUTOSIZE);

	gSocket = BOViL::comm::Socket::createSocket(BOViL::comm::eSocketType::clientTCP,"9009", _argv[2]); 

	unsigned char justABuffer[1024];
	while (true){
		cv::Mat display = bg.clone();
		char cmd = cv::waitKey(100);
		std::cout <<cmd<<std::endl;
		int axis = 0;
		float value = 0;
		if(cmd == 'w'){
			axis = 4;
			value = 1;
			cv::addWeighted( display, 0.5, w, 0.5, 0.0, display);
		}else if(cmd == 's'){
			axis = 4;
			value = -1;
			cv::addWeighted( display, 0.5, s, 0.5, 0.0, display);
		}else if(cmd == 't'){
			axis = 3;
			value = -1;
			cv::addWeighted( display, 0.5, t, 0.5, 0.0, display);
		}else if(cmd == 'l'){
			axis = 3;
			value = 1;
			cv::addWeighted( display, 0.5, l, 0.5, 0.0, display);
		}else if(cmd == '8'/*'R'*/){ // UP-->Y+
			axis = 1;
			value = 1;
			cv::addWeighted( display, 0.5, up, 0.5, 0.0, display);
		}else if(cmd == '2'/*'T'*/){ // DOWN-->Y-
			axis = 1;
			value = -1;
			cv::addWeighted( display, 0.5, down, 0.5, 0.0, display);
		}else if(cmd == '4'/*'Q'*/){ //left-->X+
			axis = 0;
			value = 1;
			cv::addWeighted( display, 0.5, left, 0.5, 0.0, display);
		}else if(cmd == '6'/*'S'*/){ // right-->X-
			axis = 0;
			value = -1;
			cv::addWeighted( display, 0.5, right, 0.5, 0.0, display);
		}


		if(cmd != -1){
			// Send query
			std::string queryMsg = "pisha";
			gSocket->sendMsg((unsigned char*) queryMsg.c_str(),queryMsg.size());
			// Wait for approval
			gSocket->receiveMsg(justABuffer, 1024);
			// Send data
			std::string dataMsg =std::to_string(axis)+","+std::to_string(value);
			gSocket->sendMsg((unsigned char*) dataMsg.c_str(),dataMsg.size());
		}
		cv::imshow("display", display);
	}
}
