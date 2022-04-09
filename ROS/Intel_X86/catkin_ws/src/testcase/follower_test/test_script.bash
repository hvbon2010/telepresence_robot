#!/bin/bash
STRING="folllower"
DATE=$(date)
echo $STRING+$DATE
FILENAME=$STRING+$DATE
python robot_trajectory.py &>"$FILENAME"
echo "SUCCESSFULL LOG DATA INTO $FILENAME"
