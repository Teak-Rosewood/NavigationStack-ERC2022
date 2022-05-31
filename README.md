# Mars Rover Manipal - ERC 2022 - Navigation Stack 

The current version of the simulation targets [ROS Noetic Ninjemys](http://wiki.ros.org/noetic/Installation/) distribution and was mainly developed and tested on [Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/).
 
## Cloning of the ERC-Navigation Repository 
 
### Prerequisites 

1. [Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/)
2. [ROS Noetic Ninjemys](http://wiki.ros.org/noetic/Installation/)

```
sudo apt-get install ros-noetic-octomap 
sudo apt-get install ros-noetic-octomap-mapping 
sudo apt-get install ros-noetic-octomap-msgs 
sudo apt-get install ros-noetic-octomap-ros 
sudo apt-get install ros-noetic-octomap-rviz-plugins 
sudo apt-get install ros-noetic-octomap-server  
sudo apt-get install libsdl-image1.2-dev  
sudo apt-get install libsdl-dev  
sudo apt-get install ros-noetic-tf2-sensor-msgs
```
Once all the dependencies have been installed:
```
git clone https://github.com/Blank-wastaken/MRM-ERC2022-NavStack.git
cd MRM-ERC2022-NavStack
catkin build
source devel/setup.bash
```
### Running the Navigation Stack 

The node for mapping and localization, performed through a blender file converted to an occupency grid and robot_localization is launched through: 
```
roslaunch rover mapping.launch
```
The node to launch move_base is done through: 
```
roslaunch rover move_base.launch
```
To publish a destination on the map click on the 2D nav goal button on rviz and select the destination position.

### Work to be done:
 
1. Working on a Docker Image for the Navigation Stack - 4th June 
2. Working on localization integration using pose from ar track data and robot_localization -4th June 
3. Working of Local planner - 4th June 
4. Working on producing a map without processing the point cloud - 4th June  
5. Working on a GUI implementation for sending Navigation Goals - 3rd Test Drive 
6. Working on sending goals in bulk with a status update and automatic probe dropping - 3rd Test Drive 
7. Setting up probe dropping - 3rd Test Drive 

## Running the Docker 
