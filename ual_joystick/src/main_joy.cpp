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

#include <joystick_controller/Joystick.h>
#include <unistd.h>

#include <simple_sockets_cpp/com/Socket.h>

int main(int _argc, char** _argv) {
	// Create an instance of Joystick
	Joystick joystick(_argv[1]);

	// Ensure that it was found and that we can use it
	if (!joystick.isFound()) {
		printf("open failed.\n");
		exit(1);
	}


	BOViL::comm::Socket* socket = BOViL::comm::Socket::createSocket(BOViL::comm::eSocketType::clientTCP,"9009", _argv[2]); 

	unsigned char justABuffer[1024];
	while (true) {
		// Restrict rate
		usleep(1000);

		// Attempt to sample an event from the joystick
		JoystickEvent event;
		if (joystick.sample(&event)) {
			if (event.isAxis()) {
				// Send query
				std::string queryMsg = "pisha";
				socket->sendMsg((unsigned char*) queryMsg.c_str(),queryMsg.size());

				// Wait for approval
				socket->receiveMsg(justABuffer, 1024);

				if(event.number == 1 || event.number == 4){
					event.value *=-1;
				}
				// Send data
				std::string dataMsg =std::to_string(int(event.number))+","+std::to_string(float(event.value)/JoystickEvent::MAX_AXES_VALUE);
				socket->sendMsg((unsigned char*) dataMsg.c_str(),dataMsg.size());
				
				printf("Axis %u is at position %d\n", event.number, event.value);
			}
		}
	}
}
