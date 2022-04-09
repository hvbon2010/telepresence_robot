# How to build source code
Install the ROS first
http://wiki.ros.org/Installation/Ubuntu

And then, go to the catkin_ws, and run `catkin_make`.

After compile successfully, 2 folders will be generated is `build` and `devel`.

# How to run
Setup environment:

`source ~/.bashrc`

`source devel/setup.bash`

Allow permission to read/write data into com port:

`sudo chmod 666 /dev/ttyACM0`

Launch ROS for standalone telepresence robot:

`roslaunch telebot_bringup robot_standalone.launch`

Launch ROS server:

`roslaunch rosbridge_server rosbridge_websocket.launch`

Launch ROS for telepresence robot with amcl mapping file:

`roslaunch telebot_bringup amcl_demo.launch map_file:=/home/bon/catkin_ws/src/telebot/telebot_bringup/map/bk_map_2505_01.yaml`
