#include "ros/ros.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "std_msgs/Empty.h"
#include "std_srvs/Trigger.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseWithCovariance.h"
#include "geometry_msgs/TransformStamped.h"
#include "robot_localization/SetPose.h"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <iostream>

// Defining Global Variables

geometry_msgs::PoseWithCovarianceStamped msg;

class ar_track_transforms
{
    private:

        ros::NodeHandle nh1;
        ros::NodeHandle nh2;
        ros::Subscriber sub_arTrack;
        ros::Publisher pub_arTrack;
        ros::Publisher pub_transform;
        ros::Publisher pub_resetZed2Odom;
        ros::Publisher pub_setPose;
        ros::ServiceClient pub_resetWheelOdom;

        int reset_odom = 0;
        int initial_transform_publisher = 1;

    public:

        ar_track_transforms(ros::NodeHandle &ar_track, ros::NodeHandle &transform_pub);

        void ar_callback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg);
        

};

ar_track_transforms::ar_track_transforms(ros::NodeHandle &ar_track, ros::NodeHandle &transform_pub)
{
    nh1 = ar_track;
    nh2 = transform_pub;

    sub_arTrack = nh1.subscribe("/ar_pose_marker", 1, &ar_track_transforms::ar_callback, this);

    pub_setPose = transform_pub.advertise<geometry_msgs::PoseWithCovarianceStamped>("/set_pose", 1);
    pub_resetZed2Odom = transform_pub.advertise<std_msgs::Empty>("/zed2/reset_odometry", 1);
}
    
void ar_track_transforms::ar_callback (const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg)
{

    // For publishing transform when AR Tags are in view 
    if (ar_msg->markers[0].header.frame_id == "map")
    {
        // publish detected transform 

        msg.pose.pose = ar_msg->markers[0].pose.pose;
        msg.header = ar_msg->markers[0].header;
        msg.pose.covariance = {0};

        // Changing the flag value 
        
    }

    // For publishing transform when AR Tags go out of view 

}

double getYaw(double x, double y, double z, double w)
{

    double yaw = atan2(2.0 * (y * z + w * x), w * w - x * x - y * y + z * z);
    double pitch = asin(-2.0 * (x * z - w * y));
    double roll = atan2(2.0 * (x * y + w * z), w * w + x * x - y * y - z * z);
    int yaw_rounded = yaw;

    return yaw_rounded + M_PI;
}

int main (int argc, char **argv)
{

    ros::init(argc, argv, "AR_tag_global_pose");
    ros::NodeHandle ar_track;
    ros::NodeHandle transform_pub;
    ros::Rate rate(30);

    // Subscribed to ar_track_alver 

    // Topics published on 

    ros::Publisher set_pose = transform_pub.advertise<geometry_msgs::PoseWithCovarianceStamped>("/set_pose", 1);
    ros::Publisher reset_zed2_odom = transform_pub.advertise<std_msgs::Empty>("/zed2/reset_odometry", 1);\

    
    // Setting up client for reseting wheel odometry 

    ros::ServiceClient reset_wheel_odom = transform_pub.serviceClient<std_srvs::Trigger>("firmware/reset_odometry");

    rate.sleep();

    while(ros::ok())
    {
        ros::spinOnce();

        // Converting recieved quaternion to recieve yaw 

        // double yaw = getYaw(msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, msg.pose.pose.orientation.z, msg.pose.pose.orientation.w);

        // if (flag_initial == 1)
        // {
        //     static tf::TransformBroadcaster br;
        //     tf::Transform transform;
        //     transform.setOrigin(tf::Vector3(msg.pose.pose.position.x, 
        //                                     msg.pose.pose.position.y, 
        //                                     0));
        //     tf::Quaternion q;
        //     q.setRPY(0, 
        //              0,
        //              yaw);
        //     transform.setRotation(q);
        //     br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));
        //     ros::Duration(5).sleep();
        // }
        
        // if (flag_initial == 0)
        // {
        //     static tf2_ros::TransformBroadcaster br;
        //     geometry_msgs::TransformStamped transform;

        //     transform.header.stamp = ros::Time::now();
        //     transform.header.frame_id = "map";
        //     transform.child_frame_id = "odom";

        //     transform.transform.translation.x = 0;
        //     transform.transform.translation.y = 0;
        //     transform.transform.translation.z = 0;

        //     tf2::Quaternion q;
        //     q.setRPY(0, 0, M_PI);
        //     transform.transform.rotation.x = q.x();
        //     transform.transform.rotation.y = q.y();
        //     transform.transform.rotation.z = q.z();
        //     transform.transform.rotation.w = q.w();

        //     br.sendTransform(transform);
        // }
        

        // Reseting odom and setting pose if AR Tag is in view

        // if (flag_pose_reset == 1)
        // {
        //     // Reseting Zed2 Odom 

        //     std_msgs::Empty empty;
        //     reset_zed2_odom.publish(empty);

        //     // Reseting wheel odom 

        //     std_srvs::Trigger trigger;
        //     reset_wheel_odom.call(trigger);

        //     // Setting pose of base_footprint 

        //     geometry_msgs::PoseWithCovarianceStamped msg1;
        //     msg1.header.frame_id = "odom";
        //     msg1.header.seq = 0;
        //     msg1.header.stamp = ros::Time::now();
        //     msg1.pose.covariance = {0};
        //     msg1.pose.pose.position.x = 0;
        //     msg1.pose.pose.position.y = 0;
        //     msg1.pose.pose.position.z = 0;
        //     msg1.pose.pose.orientation.x = 0;
        //     msg1.pose.pose.orientation.y = 0;
        //     msg1.pose.pose.orientation.z = 0;
        //     msg1.pose.pose.orientation.w = 0;
        //     set_pose.publish(msg1);
        // }
        rate.sleep();
    }
}