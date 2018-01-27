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

int main(int _argc, char **_argv) {
    grvc::utils::ArgumentParser argParser(_argc, _argv); 

    grvc::ual::UAL mUal(argParser);
    while (!mUal.isReady() && ros::ok()) {
        sleep(1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    geometry_msgs::PoseStamped pose;
    int cmd;
	double value;
    while(ros::ok()){
		std::cout << "Options: 0) takeoff; 1) land; 2) step X; 3) step Y; 4) step Z" << std::endl;
		std::cin >> cmd;
		switch(cmd){
		case 0:
			std::cout << "Desired take off altitude: "
			std::cin >> value;
			mUal.takeOff(value);
			break;
		case 1:
			mUal.land(true);
			break;
		case 2:
			std::cout << "Desired step in X: "
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.x +=value;
			mUal.goToWaypoint(pose);
			break;
		case 3:
			std::cout << "Desired step in Y: "
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.y +=value;
			mUal.goToWaypoint(pose);
			break;
		case 4:
			std::cout << "Desired step in Z: "
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.z +=value;
			mUal.goToWaypoint(pose);
			break;
		default:
			std::cout << "unknown command" << std::endl;
		}

		std::cout << "Finished command, wayting for next one" << std::endl;
    }
    
}
