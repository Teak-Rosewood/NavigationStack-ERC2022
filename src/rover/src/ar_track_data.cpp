#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseWithCovariance.h"

#include <iostream>

// Defining Global Variables

geometry_msgs::PoseWithCovarianceStamped msg;

void AR_callback (const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg)
{
    geometry_msgs::PoseWithCovariance msg1;
    msg.header = ar_msg->markers[0].header;
    auto a = ar_msg->markers[0];
    msg1.pose = a.pose.pose;
    msg1.covariance = {0};
    msg.pose = msg1;
}
int main (int argc, char **argv)
{

    ros::init(argc, argv, "AR_Track_Correction_Node");
    ros::NodeHandle n;
    ros::Rate rate(10);
    ros::Subscriber subAR_Track = n.subscribe("/ar_pose_marker", 1, &AR_callback);
    ros::Publisher pub = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/ar_pose_corrected", 1);
    rate.sleep();

    while(ros::ok())
    {
        ros::spinOnce();
        pub.publish(msg);
        rate.sleep();
    }

}