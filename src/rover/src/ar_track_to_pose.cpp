#include <stdio.h>
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

points circlesIntersections(circle a, circle b)
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
int main()
{
    std::map<int, ar_tag> ar_tags;

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

    // Extracting radius from pose published and AR Tag number

    // Extraction of rover orientation data

    // Finding the position of rover from pose 

    // Eliminating one of the pose values using orientation


    std::cout << ar_tags.at(2).x;
    return 0;
}