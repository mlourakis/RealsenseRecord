#ifndef CV_HELPERS
#define CV_HELPERS

#include <vector>

#include "opencv2/opencv.hpp"
using namespace cv;

#include <librealsense2/rs.hpp>                 // Include RealSense Cross Platform API
#include <librealsense2/rs_advanced_mode.hpp>   // Load an advanced camera configuraiton
#include <boost/filesystem.hpp>                 // Create directories to store the data
#include <math.h>

using namespace rs2;
using namespace std;

// MACROS to notify about device events using a sound
#define BEEP_ON  { int out_sys = system("canberra-gtk-play -f /files/Projects/UnderDev/roboslam/libraries/media/start_sound.ogg"); }
#define BEEP_OFF { int out_sys = system("canberra-gtk-play -f /files/Projects/UnderDev/roboslam/libraries/media/prompt.ogg"); }

// Retrieve combinations of supported RGB/depth resolutions + frame rates, the rgb and depth sensor indices,
// as well as accel/gyro frequencies
bool retrieve_res_and_rate_combos(const rs2::device& dev, const rs2_format rgb_fmt, const rs2_format depth_fmt,
        std::map<std::pair<int, int>, std::vector<int>>& rgb_res_n_fps, int& rgb_idx,
        std::map<std::pair<int, int>, std::vector<int>>& depth_res_n_fps, int& dpth_idx,
        std::pair<std::vector<int>, std::vector<int>>& accel_n_gyro);

// Platform independent way to create a directory, if it does not exist
// ref: https://stackoverflow.com/questions/9235679/create-a-directory-if-it-doesnt-exist
void create_dir_if_not_exists(std::string directory);

// Retrieve the sensor name
std::string get_sensor_name(const rs2::sensor& sensor);
// Get the depth sensor scale
float get_depth_scale(rs2::device dev);
// Check if the device supports an IMU
bool check_imu_is_supported();

struct frmGyro {
public:
    frmGyro(){};
    frmGyro(double ts, rs2_vector m)
    : _ts(ts), _m(m) {};

    double _ts;
    rs2_vector _m;
};
struct frmAcc {
public:
    frmAcc(){};
    frmAcc(double ts, rs2_vector m)
    : _ts(ts), _m(m) {};
    
    double _ts;
    rs2_vector _m;
};
struct frmRGB {
public:
    frmRGB(){};
    frmRGB(double ts, cv::Mat m)
    : _ts(ts), _m(m.clone()) {

    };
    
    double _ts;
    cv::Mat _m;
};
struct frmDepth {
public:
    frmDepth(){};
    frmDepth(double ts, cv::Mat m)
    : _ts(ts), _m(m.clone()){};
    
    double _ts;
    cv::Mat _m;
};

#endif
