//
//
//
//
//


#include <ros/ros.h>
#include <uav_abstraction_layer/ual.h>
#include <iostream>
#include <string>
#include <fstream>

int main(int _argc, char **_argv) {

    grvc::ual::UAL mUal(_argc, _argv);
    while (!mUal.isReady() && ros::ok()) {
        sleep(1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    geometry_msgs::PoseStamped pose;
    int cmd;
	double value;
    while(ros::ok()){
		std::cout << "Options: 0) takeoff; 1) land; 2) step X; 3) step Y; 4) step Z; 5) WP1; 6) WP2; 7) WP3; 8) Show Pose; 9) Custom Pose" << std::endl;
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
			float x, y, z;
			x = -1.902528;
			y = 3.118153;
			z = 2.361217;
			std::cout << "Go to WP1: " << x << " , " << y << " , " << z << std::endl;
			std::cout << "Are you sure? press 1 ";
			int val = 0;
			std::cin >> val;
			if(val != 1){
				std::cout << "Skipping" << std::endl;
				continue;
			}	
			auto targetPose = mUal.pose();
			targetPose.pose.position.x =x;
			targetPose.pose.position.y =y;
			targetPose.pose.position.z =z;
			mUal.goToWaypoint(targetPose);
			break;
		}
		case 6:
		{	
			float x, y, z;
			x = 1.192234;
			y = 3.1057634;
			z = 2.43994;
			std::cout << "Go to WP2: "<< x << " , " << y << " , " << z << std::endl;
			std::cout << "Are you sure? press 1 ";
			int val = 0;
			std::cin >> val;
			if(val != 1){
				std::cout << "Skipping" << std::endl;
				continue;
			}	
			auto targetPose = mUal.pose();
			targetPose.pose.position.x =x;
			targetPose.pose.position.y =y;
			targetPose.pose.position.z =z;
			mUal.goToWaypoint(targetPose);
			break;
		}
		case 7:
		{	
			float x, y, z;
			x = 2.245302;
			y = 3.154002;
			z = 2.445038;
			std::cout << "Go to WP3: " << x << " , " << y << " , " << z << std::endl;
			std::cout << "Are you sure? press 1 ";
			int val = 0;
			std::cin >> val;
			if(val != 1){
				std::cout << "Skipping" << std::endl;
				continue;
			}	
			auto targetPose = mUal.pose();
			targetPose.pose.position.x =x;
			targetPose.pose.position.y =y;
			targetPose.pose.position.z =z;
			mUal.goToWaypoint(targetPose);
			break;
		}
		case 8:
		{
			std::cout << "Show pose from UAL" << std::endl;
			pose = mUal.pose();
			std::cout << "X: " << pose.pose.position.x << " | " << "Y: " << pose.pose.position.y << " | " << "Z: " << pose.pose.position.z << std::endl;
			break;
		}
		case 9:
		{
			float x, y, z;
			std::cout << "Custom pose: ";
			std::cout << "x: "; std::cin >> x;
			std::cout << "y: "; std::cin >> y;
			std::cout << "z: "; std::cin >> z;
			pose = mUal.pose();
			std::cout << "You wrote: " << x << ", " << y << ", " << z << std::endl;
			std::cout << "And current pose is: " << pose.pose.position.x << ", " << pose.pose.position.y << ", " << pose.pose.position.z << std::endl;
			std::cout << "Are you sure? press 1 ";
			int val = 0;
			std::cin >> val;
			if(val != 1){
				std::cout << "Skipping" << std::endl;
				continue;
			}			
			std::cout << "Moving to: " << x << ", " << y << ", " << z << std::endl;
			auto targetPose = mUal.pose();
			targetPose.pose.position.x =x;
			targetPose.pose.position.y =y;
			targetPose.pose.position.z =z;
			mUal.goToWaypoint(targetPose);
			break;
		}
		default:
			std::cout << "unknown command" << std::endl;
		}

		std::cout << "Finished command, wayting for next one" << std::endl;
    }
    
}
