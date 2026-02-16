### realsense_record_publisher
Use this package to publish Realsense Record datasets in ROS2. Currently, the RGB and depth frames are supported.

#### Synchronizing data frames
If your data is not synchronized, you first must run the synchronization script by following these steps:

1. First make it executable:
```
chmod +x ${ROS_WORKSPACE}/src/realsense_record_publisher/scripts/assoc_rgbdi.py
```
2. And then run it:
```
ros2 run realsense_record_publisher assoc_rgbdi.py ${DATASET_DIRECTORY}
```
The script will generate the synchronized indexes with the \_aligned suffix in the dataset directory.

#### Installation
To install the package, simply copy the realsense_record_publisher directory into your ROS2 workspace and build it.

#### Running the publisher
After the dataset is synchronized, you can launch the publisher:
```
ros2 launch realsense_record_publisher realsense_record_publisher_launch.py
```
The parameters of the publisher are located in the file ```launch/realsense_record_publisher_launch.py```.
In particular, ```start_frame``` allows publishing to begin from an arbitrary frame index rather than the first one.

#### Using simulated time
The publisher can be instructed to use the timestamps marking when data were recorded instead of the current time. To do this, set ``use_sim_time`` with
```
ros2 launch realsense_record_publisher realsense_record_publisher_launch.py use_sim_time:=false
```
By default, ``/clock`` is also published in this case. To disable it, add the launch parameter ``publish_clock:=true``.
Note that all subscribers should also set ``use_sim_time``.
