#include "ros/ros.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "nav_msgs/Odometry.h"

#include <math.h>
#include <iostream>
#include <ostream>
#include <map>

struct circle
{
    double x;
    double y;
    double r;
};
struct points
{
    double x1;
    double y1;
    double x2;
    double y2;
};
struct ar_tag
{
    double x;
    double y;
    double z;
};

class poseDetection
{
public:
    ros::Publisher pose_pub;
    std::map<int, ar_tag> ar_tags;

    poseDetection(ros::NodeHandle n)
    {
        pose_pub = n.advertise<nav_msgs::Odometry>("ar_track/pose", 1);

        // Inserting AR tag coordinates on the map

        ar_tags.insert(std::pair<int, ar_tag>(1, {9.80, 0.00, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(2, {9.80, 3.5, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(3, {34.00, 1.50, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(4, {23.63, -4.62, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(5, {10.27, 9.76, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(6, {10.10, -21.38, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(7, {5.44, -15.17, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(8, {31.00, -9.13, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(9, {18.37, 11.00, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(10, {1.36, 9.60, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(11, {17.00, -22.46, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(12, {19.63, -0.02, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(13, {18.29, -13.90, 1}));
        ar_tags.insert(std::pair<int, ar_tag>(14, {0, 0, 0}));
        ar_tags.insert(std::pair<int, ar_tag>(15, {3.02, -17.34, 1}));
        //std::cout << ar_tags.at(2).x
    }

    void ar_callback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg);
    void odom_callback(const nav_msgs::Odometry::ConstPtr &odom_msg);

private:

    ar_track_alvar_msgs::AlvarMarkers arTrackMsg;
    circle tag1, tag2;
    points final_points, tag_1_dist, tag_2_dist;
    float odom_x, odom_y;
    nav_msgs::Odometry final_odom;

    void getARTrackData();
    float getDistance(points a);
    points circlesIntersections (circle a, circle b);
    void posePublisher (points a);
    
};

void poseDetection::odom_callback(const nav_msgs::Odometry::ConstPtr &odom_msg)
{
    odom_x = odom_msg->pose.pose.position.x;
    odom_y = odom_msg->pose.pose.position.y;
}

void poseDetection::ar_callback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &ar_msg)
{
    tag1.x = ar_tags.at(ar_msg->markers[0].id).x;
    tag1.y = ar_tags.at(ar_msg->markers[0].id).y;
    tag2.x = ar_tags.at(ar_msg->markers[1].id).x;
    tag2.y = ar_tags.at(ar_msg->markers[1].id).y;

    points temp;
    temp.x1 = tag1.x;
    temp.y1 = tag1.y;
    temp.x2 = ar_msg->markers[0].pose.pose.position.x;
    temp.y2 = ar_msg->markers[0].pose.pose.position.y;

    tag1.r = getDistance(temp);

    temp.x1 = tag2.x;
    temp.y1 = tag2.y;
    temp.x2 = ar_msg->markers[1].pose.pose.position.x;
    temp.y2 = ar_msg->markers[2].pose.pose.position.y;

    tag2.r = getDistance(temp);
    

    final_points = circlesIntersections(tag1, tag2);

    posePublisher(final_points);
}

float poseDetection::getDistance(points a)
{
    float distance;
    distance = sqrt(pow((a.x1-a.x2), 2) + pow((a.y1 - a.y2), 2));

    return distance;
}
points poseDetection::circlesIntersections(circle a, circle b)
{
    points intersection;

    double circle_distance = sqrt(pow((a.x - b.x), 2) - pow((a.y * b.y), 2));

    intersection.x1 = ((a.x + b.x) / 2) +
                      ((pow(a.r, 2) - pow(b.r, 2)) * (b.x - a.x) / (2 * pow(circle_distance, 2))) +
                      (sqrt((2 * (pow(a.r, 2) + pow(b.r, 2)) / pow(circle_distance, 2)) -
                            (pow((pow(a.r, 2) - pow(b.r, 2)), 2) / pow(circle_distance, 4)) - 1) *
                       (b.y - a.y) / 2);
    intersection.x2 = ((a.x + b.x) / 2) +
                      ((pow(a.r, 2) - pow(b.r, 2)) * (b.x - a.x) / (2 * pow(circle_distance, 2))) -
                      (sqrt((2 * (pow(a.r, 2) + pow(b.r, 2)) / pow(circle_distance, 2)) -
                            (pow((pow(a.r, 2) - pow(b.r, 2)), 2) / pow(circle_distance, 4)) - 1) *
                       (b.y - a.y) / 2);
    intersection.y1 = ((a.y + b.y) / 2) +
                      ((pow(a.r, 2) - pow(b.r, 2)) * (b.y - a.y) / (2 * pow(circle_distance, 2))) +
                      (sqrt((2 * (pow(a.r, 2) + pow(b.r, 2)) / pow(circle_distance, 2)) -
                            (pow((pow(a.r, 2) - pow(b.r, 2)), 2) / pow(circle_distance, 4)) - 1) *
                       (a.x - b.x) / 2);
    intersection.y2 = ((a.y + b.y) / 2) +
                      ((pow(a.r, 2) - pow(b.r, 2)) * (b.y - a.y) / (2 * pow(circle_distance, 2))) -
                      (sqrt((2 * (pow(a.r, 2) + pow(b.r, 2)) / pow(circle_distance, 2)) -
                            (pow((pow(a.r, 2) - pow(b.r, 2)), 2) / pow(circle_distance, 4)) - 1) *
                       (a.x - b.x) / 2);

    return intersection;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "arTrack_pose");
    ros::NodeHandle n;
    ros::Rate rate(30);

    poseDetection poseObject(n);

    ros::Subscriber sub = n.subscribe("/ar_pose_marker", 1, &poseDetection::ar_callback, &poseObject);
    ros::Subscriber sub = n.subscribe("/odometry/filtered", 1, &poseDetection::odom_callback, &poseObject);

    rate.sleep();

    ros::spin();
}