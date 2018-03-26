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
		std::cout << "Options: 0) takeoff; 1) land; 2) step X; 3) step Y; 4) step Z; 5) circle; 6) eight shape; 7) show pose" << std::endl;
		std::cin >> cmd;
		switch(cmd){
		case 0:
			std::cout << "Desired take off altitude: ";
			std::cin >> value;
			mUal.takeOff(value);
			break;
		case 1:
			mUal.land(true);
			break;
		case 2:
			std::cout << "Desired step in X: ";
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.x +=value;
			mUal.goToWaypoint(pose);
			break;
		case 3:
			std::cout << "Desired step in Y: ";
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.y +=value;
			mUal.goToWaypoint(pose);
			break;
		case 4:
			std::cout << "Desired step in Z: ";
			std::cin >> value;
			pose = mUal.pose();
			pose.pose.position.z +=value;
			mUal.goToWaypoint(pose);
			break;
		case 5:
		{
			std::cout << "Radius of circle: ";
			std::cin >> value;
			std::vector<geometry_msgs::PoseStamped> poses;
			pose = mUal.pose();
			for(unsigned i = 0; i < 16; i++){
				auto cirPose = pose;
				cirPose.pose.position.x += value*cos(2*M_PI/16*i);
				cirPose.pose.position.y += value*sin(2*M_PI/16*i);
				poses.push_back(cirPose);
			}
			for(auto &pos: poses){
				mUal.goToWaypoint(pos);
			}
		}
		case 6:
		{
			std::cout << "Radius of eight: ";
			std::cin >> value;
			std::vector<geometry_msgs::PoseStamped> poses;
			pose = mUal.pose();
			for(unsigned i = 0; i < 16; i++){
				auto eigPose = pose;
				eigPose.pose.position.x += value*sin(2*M_PI/16*i);
				eigPose.pose.position.y += value*sin(2*M_PI/16*i)*cos(2*M_PI/16*i);
				poses.push_back(eigPose);
			}
			for(auto &pos: poses){
				mUal.goToWaypoint(pos);
			}
		}
		case 7:
		{
			std::cout << "Show pose from UAL" << std::endl;
			pose = mUal.pose();
			std::cout << "X: " << pose.pose.position.x << " | " << "Y: " << pose.pose.position.x << " | " << "Z: " << pose.pose.position.x << std::endl;
		}
		default:
			std::cout << "unknown command" << std::endl;
		}

		std::cout << "Finished command, wayting for next one" << std::endl;
    }
    
}
