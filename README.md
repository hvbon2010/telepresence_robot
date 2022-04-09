# Introduction
Telepresense robot support for remote controlling, video streaming. So, it is used to
monitor factories, monitor children, tour guides. And especially, it is an extremely effective
healthcare assistant for the elderly.

# Structure of this project
## Fimware
Initialization, communication with hardwares (sensors, motor driver) to collect data and send to
ROS, excute the system command by controlling the motor drivers.

## ROS
The Robot Operating System (ROS) is a set of software libraries and tools that help you build robot applications. From drivers to state-of-the-art algorithms, and with powerful developer tools, ROS has what you need for your next robotics project. And it's all open source.

Refer: https://www.ros.org/

ROS is used in this robot to collect odometry data (from encoder, MPU6050, kinect camera) for SLAM (Simultaneous Localization and Mapping) to localization and mapping this robot on the created map. And then, ROS navigation package will takes in the above information and a goal pose to output the velocity commands into a mobile base.

http://wiki.ros.org/gmapping
http://wiki.ros.org/navigation

The telepresense robot is refer to chefbot: https://github.com/qboticslabs/Chefbot_ROS_pkg

## Server
To run the remote controlling and video streaming feature, we need to use a server.
We use WebRTC to run this features.

https://webrtc.org/

# Demo video
https://www.youtube.com/watch?v=L0Hp8z0R1nA
