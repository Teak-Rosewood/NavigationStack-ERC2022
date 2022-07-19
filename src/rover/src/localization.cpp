#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"

#include "std_srvs/Trigger.h"
#include "std_msgs/Empty.h"

#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>

#include <iostream>
#include <math.h>

class localization 
{
    public:

        geometry_msgs::PoseWithCovarianceStamped finalMsg;

        void wheelEncoderCallback ();
        void IMUCallback();
        void cmd_velCallback();
    
    private:

        geometry_msgs::PoseWithCovarianceStamped encoderMsg;
        geometry_msgs::Twist cmd_velMsg;
        sensor_msgs::Imu imuMsg;

        double getYaw(double x, double y, double z, double w);
        void fuseSensorData ();
        void publishTransforms ();
};

double localization::getYaw(double x, double y, double z, double w)
{

    double yaw = atan2(2.0 * (y * z + w * x), w * w - x * x - y * y + z * z);
    double pitch = asin(-2.0 * (x * z - w * y));
    double roll = atan2(2.0 * (x * y + w * z), w * w + x * x - y * y - z * z);
    int yaw_rounded = yaw;

    return yaw_rounded + M_PI;
}

int main (int argc, char** argv)
{
    ros::init (argc, argv, "localizationNode");
    ros::NodeHandle ar_track;
    ros::Rate rate(60);

    // Subscribers 
    
    ros::Subscriber wheelEncoder_sub = 
}