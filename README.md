# Mars Rover Manipal - ERC 2022 - Navigation Stack 

The current version of the simulation targets [ROS Noetic Ninjemys](http://wiki.ros.org/noetic/Installation/) distribution and was mainly developed and tested on [Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/).
 
## Cloning of the ERC-Navigation Repository 
 
### Prerequisites 

1. [Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/)
2. [ROS Noetic Ninjemys](http://wiki.ros.org/noetic/Installation/)

The tools necessary to build this project can be installed with apt:
```
sudo apt-get install libgeographic-dev ros-noetic-geographic-msgs
sudo apt install python3-rosdep python3-catkin-tools
```
 
### Building 
 
To clone the repository:
```
git clone https://github.com/Blank-wastaken/MRM-ERC2022-NavStack.git
```
Use the `rosdep` tool to install any missing dependencies. If you are running `rosdep` for the first time, you might have to run:
```
sudo rosdep init
```
first. Then, to install the dependencies, type:
```
rosdep update
sudo apt update
rosdep install --rosdistro noetic --from-paths src -iy
```
Now, use the `catkin` tool to build the workspace:
```sh
catkin config --extend /opt/ros/noetic
catkin build
source devel/setup.bash
```

### Running the Navigation Stack 

launching the leo rover on gazebo and rviz can be done using: 
```
roslaunch rover gazebo_rviz.launch
```
The following are the launch files to individually launch used packages: 
```
# Launching robot_localization nodes:
 
roslaunch rover localization.launch
 
# Launching global mapping nodes:
 
roslaunch rover mapping.launch
 
# Launching ar_track_alvar node:
 
roslaunch rover ar_track_alvar.launch

# Launching move base:
 
roslaunch rover move_base.launch 
```
As an alternative the entire navigation stack can be launched:
'''
roslaunch navigation.launch
'''
To publish a destination on the map click on the 2D nav goal button on rviz and select the destination position.

### Work to be done:
 
1. Working on a Docker Image for the Navigation Stack - 4th June (done)
2. Working on localization integration using pose from ar track data and robot_localization -4th June 
3. Working of Local planner - 4th June 
4. Working on producing a map without processing the point cloud - 4th June  
5. Working on a GUI implementation for sending Navigation Goals - 3rd Test Drive 
6. Working on sending goals in bulk with a status update and automatic probe dropping - 3rd Test Drive 
7. Setting up probe dropping - 3rd Test Drive 

## Running the Docker 
 

