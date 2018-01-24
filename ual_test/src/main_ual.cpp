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
    while(ros::ok()){
	std::cin >> cmd;
	switch(cmd){
	case 0:
	    mUal.takeOff(3.0);
	    break;
	case 1:
	    mUal.land(true);
	    break;
	case 2:
	    pose = mUal.pose();
	    pose.pose.position.x +=2;
	    mUal.goToWaypoint(pose);
	    break;
	case 3:
	    pose = mUal.pose();
	    pose.pose.position.x -=2;
	    mUal.goToWaypoint(pose);
	    break;
	case 4:
	    pose = mUal.pose();
	    pose.pose.position.y +=2;
	    mUal.goToWaypoint(pose);
	    break;
	case 5:
	    pose = mUal.pose();
	    pose.pose.position.y -=2;
	    mUal.goToWaypoint(pose);
	    break;
	case 6:
	    pose = mUal.pose();
	    pose.pose.position.z +=2;
	    mUal.goToWaypoint(pose);
	    break;
	case 7:
	    pose = mUal.pose();
	    pose.pose.position.z -=2;
	    mUal.goToWaypoint(pose);
	    break;
	default:
	    std::cout << "unknown command" << std::endl;
	}

	std::cout << "Finished command, wayting for next one" << std::endl;
    }
    
}
