#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include <iostream>

int main(int argc,char **argv){
    float x_pose,y_pose,z_pose,x,y,z,w;
    std::cout<<"Enter Pose(x,y): "<<std::endl;
    std::cin>>x_pose>>y_pose;
    std::cout<<"Enter Orientation(z,w): "<<std::endl;
    std::cin>>z>>w;
    ros::init(argc,argv,"movebase_goal");
    ros::NodeHandle node;
    ros::Publisher pub = node.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal",10);
    geometry_msgs::PoseStamped msg;
    msg.pose.position.x = x_pose;
    msg.pose.position.y = y_pose;
    msg.pose.orientation.z = z;
    msg.pose.orientation.w = w;
    msg.header.frame_id="map";
    ros::Rate rate(10);
    while(ros::ok()){
        pub.publish(msg);
        ros::spinOnce();
        rate.sleep();
    }
}