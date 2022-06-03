#include "ros/ros.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "std_msgs/Empty.h"
#include "std_srvs/Trigger.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseWithCovariance.h"
#include <tf/transform_broadcaster.h>
#include <iostream>


// Defining Global Variables

geometry_msgs::PoseWithCovarianceStamped msg;
int flag_pose_reset = 0;
int flag_initial = 0;

void AR_callback (const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg)
{

    // Publishing Transform when AR Tags are in view 

    if (ar_msg->header.frame_id == "map")
    {
        // publish detected transform 

        msg.pose.pose = ar_msg->markers[0].pose.pose;
        msg.header = ar_msg->markers[0].header;
        msg.pose.covariance = {0};
        static tf::TransformBroadcaster br;
        tf::Transform transform;
        transform.setOrigin(tf::Vector3(msg.pose.pose.position.x, 
                                        msg.pose.pose.position.y, 
                                        msg.pose.pose.position.z));
        tf::Quaternion q;
        q.setValue(msg.pose.pose.orientation.x, 
                    msg.pose.pose.orientation.y,
                    msg.pose.pose.orientation.z,
                    msg.pose.pose.orientation.w);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));

        // Changing the flag value 
        
        flag_pose_reset = 1;
        flag_initial = 1;
    }

    // Publishing Transform when AR Tags are not visible from starting position
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

    // Publishing Transform when AR Tags go out of view 

    else if (flag_initial == 1)
    {
        // publish previously published transforms
        static tf::TransformBroadcaster br;
        tf::Transform transform;
        transform.setOrigin(tf::Vector3(msg.pose.pose.position.x, 
                                        msg.pose.pose.position.y, 
                                        msg.pose.pose.position.z));
        tf::Quaternion q;
        q.setValue(msg.pose.pose.orientation.x, 
                    msg.pose.pose.orientation.y,
                    msg.pose.pose.orientation.z,
                    msg.pose.pose.orientation.w);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));

        flag_pose_reset = 0;
    }
}
int main (int argc, char **argv)
{

    ros::init(argc, argv, "AR_tag_global_pose");
    ros::NodeHandle n;
    ros::Rate rate(10);

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

            geometry_msgs::PoseWithCovarianceStamped msg1;
            msg1.header.frame_id = "odom";
            msg1.header.seq = 0;
            msg1.header.stamp = ros::Time::now();
            msg1.pose.covariance = {0};
            msg1.pose.pose.position.x = 0;
            msg1.pose.pose.position.y = 0;
            msg1.pose.pose.position.z = 0;
            msg1.pose.pose.orientation.x = 0;
            msg1.pose.pose.orientation.y = 0;
            msg1.pose.pose.orientation.z = 1;
            msg1.pose.pose.orientation.w = 0.0000363;
            set_pose.call(msg);
        }

        rate.sleep();
    }

}