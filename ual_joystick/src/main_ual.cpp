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
#include <simple_sockets_cpp/com/Socket.h>

int main(int _argc, char **_argv) {

    BOViL::comm::Socket *socket = BOViL::comm::Socket::createSocket(BOViL::comm::eSocketType::serverTCP, "9009", "0.0.0.0");

   if(socket->getSocketDescriptor() == -1){
	std::cout << "Failed socket creation" << std::endl;
   	return -1;
   }

    grvc::utils::ArgumentParser argParser(_argc, _argv); 

    grvc::ual::UAL mUal(argParser);
    while (!mUal.isReady() && ros::ok()) {
        sleep(1);
    }


    geometry_msgs::TwistStamped targetSpeed;   
    auto tReception = std::chrono::high_resolution_clock::now();
    bool landSignal = false;
    bool takeoffSignal = false;	
    bool isLanded = true;
    std::thread socketThread([&](){
	unsigned char buffer[1024];
	while(ros::ok() & socket->getSocketDescriptor() != -1){
		// wait for query
 		int len = socket->receiveMsg(buffer, 1024);
		// Send ok
		std::string msg = "ok";
		socket->sendMsg((unsigned char*) msg.c_str(), msg.size());
		// receive data
 		len = socket->receiveMsg(buffer, 1024);
		//std::cout << buffer << std::endl;
		tReception = std::chrono::high_resolution_clock::now();		
		std::string data((char*)buffer, len);
		int axis = atoi(data.substr(0,data.find(",")).c_str());
		float value = atof(data.substr(data.find(",")).c_str()+1);
		
		//std::cout << "Received event from joy. Axis: " << axis << " and value: " << value << std::endl;

		if(true){
			switch(axis){
			case 0: // X
				if(fabs(value) > 0.3)
					targetSpeed.twist.linear.x = value;
				else 
					targetSpeed.twist.linear.x = 0;
				break;
			case 1: // Y
				if(fabs(value) > 0.3)
					targetSpeed.twist.linear.y = value;
				else 
					targetSpeed.twist.linear.y = 0;
				break;
			case 3: // land/takeoff
				std::cout<< value << std::endl;
				if(value > 0.9){
					landSignal = true;
					std::cout << "Received land signal" << std::endl;
				}
				else if(-0.9 > value){
					takeoffSignal = true;
					std::cout << "Received takeoff signal" << std::endl;
				}
				else{
					landSignal = false;
					takeoffSignal = false;				
				}
					// Land
				break;
			case 4: // z
				if(fabs(value) > 0.3)
					targetSpeed.twist.linear.z = value;
				else 
					targetSpeed.twist.linear.z = 0;
				break;
			}
		}
		//std::cout << targetSpeed << std::endl;
    		
	}
    });

    while(!takeoffSignal){
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    landSignal = false;
    takeoffSignal = false;	
    bool hold = false;
    geometry_msgs::PoseStamped holdPose;
    while(ros::ok() && !landSignal){
		auto timeSinceLast = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tReception).count();
		if(timeSinceLast < 500){
			hold = false;
			targetSpeed.header.stamp = ros::Time::now();
			mUal.setVelocity(targetSpeed);
			//std::cout << targetSpeed << std::endl;
		
		}else{
    		targetSpeed = geometry_msgs::TwistStamped();   
			if(!hold){
				holdPose = mUal.pose();
				hold = true;
			}
		}

    	if(hold){
			mUal.goToWaypoint(holdPose, false);
		}
       std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }


    std::this_thread::sleep_for(std::chrono::seconds(1));
    mUal.land();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}
