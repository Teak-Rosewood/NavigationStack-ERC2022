#include "ros/ros.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "std_msgs/Empty.h"
#include "std_srvs/Trigger.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseWithCovariance.h"
#include "robot_localization/SetPose.h"
#include <tf/transform_broadcaster.h>
#include <iostream>


// Defining Global Variables

geometry_msgs::PoseWithCovarianceStamped msg;
int flag_pose_reset = 0;
int flag_initial = 0;

void AR_callback (const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg)
{

    // For publishing transform when AR Tags are in view 

    std::cout << ar_msg->header.frame_id;
    if (ar_msg->markers[0].header.frame_id == "map")
    {
        // publish detected transform 

        msg.pose.pose = ar_msg->markers[0].pose.pose;
        msg.header = ar_msg->markers[0].header;
        msg.pose.covariance = {0};

        // Changing the flag value 
        
        flag_pose_reset = 1;
        flag_initial = 1;
    }

    // For publishing transform when AR Tags go out of view 

    else if (flag_initial == 1)
    {
        flag_pose_reset = 0;
    }
}
int main (int argc, char **argv)
{

    ros::init(argc, argv, "AR_tag_global_pose");
    ros::NodeHandle n;
    ros::Rate rate(60);

    // Subscribed to ar_track_alver 

    ros::Subscriber subAR_Track = n.subscribe("/ar_pose_marker", 1, &AR_callback);

    // Topics published on 

    ros::Publisher ar_tag = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/ar_pose", 1);
    ros::Publisher reset_zed2_odom = n.advertise<std_msgs::Empty>("/zed2/reset_odometry", 1);
    
    // Setting up client for reseting wheel odometry 

    ros::ServiceClient reset_wheel_odom = n.serviceClient<std_srvs::Trigger>("firmware/reset_odometry");
    ros::ServiceClient set_pose = n.serviceClient<std_srvs::Trigger>("/set_pose");

    rate.sleep();

    while(ros::ok())
    {
        ros::spinOnce();

        ar_tag.publish(msg); 

        if (flag_initial == 1)
        {
            static tf::TransformBroadcaster br;
            tf::Transform transform;
            transform.setOrigin(tf::Vector3(msg.pose.pose.position.x, 
                                            msg.pose.pose.position.y, 
                                            0));
            tf::Quaternion q;
            q.setValue(0, 
                        0,
                        msg.pose.pose.orientation.z,
                        msg.pose.pose.orientation.w);
            transform.setRotation(q);
            br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));
        }
        
        else if (flag_initial == 0)
        {
            static tf::TransformBroadcaster br;
            tf::Transform transform;
            transform.setOrigin( tf::Vector3(0, 0, 0));
            tf::Quaternion q;
            q.setValue(0, 0, 1, 0.0000363);
            transform.setRotation(q);
            br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));
        }

        // Reseting odom and setting pose if AR Tag is in view

        if (flag_pose_reset == 1)
        {
            // Reseting Zed2 Odom 

            std_msgs::Empty empty;
            reset_zed2_odom.publish(empty);

            // Reseting wheel odom 

            std_srvs::Trigger trigger;
            reset_wheel_odom.call(trigger);

            // Setting pose of base_footprint 

            robot_localization::SetPose msg1;
            msg1.request.pose.header.frame_id = "odom";
            msg1.request.pose.header.seq = 0;
            msg1.request.pose.header.stamp = ros::Time::now();
            msg1.request.pose.pose.covariance = {0};
            msg1.request.pose.pose.pose.position.x = 0;
            msg1.request.pose.pose.pose.position.y = 0;
            msg1.request.pose.pose.pose.position.z = 0;
            msg1.request.pose.pose.pose.orientation.x = 0;
            msg1.request.pose.pose.pose.orientation.y = 0;
            msg1.request.pose.pose.pose.orientation.z = 0;
            msg1.request.pose.pose.pose.orientation.w = 0;
            set_pose.call(msg1);
        }
        rate.sleep();
    }
}