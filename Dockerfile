FROM ghcr.io/europeanroverchallenge/erc-remote-image-base:latest

# Install additional packages
RUN apt-get update && apt-get -y upgrade && apt-get -y install \
  tmux \
  && rm -rf /var/lib/apt/lists/*

# Copy packages and build the workspace
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /catkin_ws
COPY src ./src
RUN apt-get update \
  && rosdep update \
  && rosdep install --from-paths src -iy \
  && rm -rf /var/lib/apt/lists/* \
  && apt-get install libgeographic-dev ros-noetic-geographic-msgs 
  # && apt-get install ros-noetic-octomap \
  # && apt-get install ros-noetic-octomap-mapping \
  # && apt-get install ros-noetic-octomap-msgs \
  # && apt-get install ros-noetic-octomap-ros \
  # && apt-get install ros-noetic-octomap-rviz-plugins \
  # && apt-get install ros-noetic-octomap-server \
  # && apt-get install libsdl-image1.2-dev \
  # && apt-get install libsdl-dev \
  # && apt-get install ros-noetic-tf2-sensor-msgs 
RUN catkin config --extend /opt/ros/noetic && catkin build --no-status 

# Automatically source the workspace when starting a bash session
RUN echo "source /catkin_ws/devel/setup.bash" >> /etc/bash.bashrc
