#!/bin/bash
echo "hello, $USER. I wish to list some files of yours"
#export ROS_MASTER_URI=http://192.169.43.69:11311
source ~/.bashrc
source devel/setup.bash
sudo chmod 666 /dev/ttyACM0
roslaunch telebot_bringup robot_standalone.launch &
roslaunch rosbridge_server rosbridge_websocket.launch &
roslaunch telebot_bringup amcl_demo.launch map_file:=/home/bon/catkin_ws/src/telebot/telebot_bringup/map/bk_map_2505_01.yaml&
#roslaunch chefbot_bringup view_navigation.launch &
