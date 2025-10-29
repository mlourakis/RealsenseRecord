#### Realsense Record ROS2 component

The directory includes a ROS2 publisher package for Realsense Record.
Move ``realsense_record_publisher`` into a directory where colcon can find it (e.g. ``ros_ws/src/``) and build normally.


To publish already recorded data stored in ``/your/data/path``, use
```
ros2 launch realsense_record_publisher realsense_record_publisher_launch.py dataset_directory:=/your/data/path
```
