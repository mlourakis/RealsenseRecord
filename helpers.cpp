#include "helpers.h"

bool retrieve_res_and_rate_combos(const rs2::device& dev, const rs2_format rgb_fmt, const rs2_format depth_fmt,
        std::map<std::pair<int, int>, std::vector<int>>& rgb_res_n_fps, int& rgb_idx,
        std::map<std::pair<int, int>, std::vector<int>>& depth_res_n_fps, int& dpth_idx,
        std::pair<std::vector<int>, std::vector<int>>& accel_n_gyro)
{
    // Clear outputs
    rgb_res_n_fps.clear();
    depth_res_n_fps.clear();
    accel_n_gyro = std::make_pair(std::vector<int>{}, std::vector<int>{});
    rgb_idx = dpth_idx = -1;

    try {
        // Enumerate sensors on the provided device
        std::vector<rs2::sensor> sensors = dev.query_sensors();

        for (size_t i = 0; i < sensors.size(); ++i) {
            // Enumerate all supported stream profiles for this sensor
            for (rs2::stream_profile profile : sensors[i].get_stream_profiles()) {
                // Try to cast to video stream profile
                if (rs2::video_stream_profile video_profile = profile.as<rs2::video_stream_profile>()) {
                    auto type = video_profile.stream_type();
                    int fps = video_profile.fps();

                    if (type == RS2_STREAM_COLOR && video_profile.format() == rgb_fmt) {
                        rgb_res_n_fps[{video_profile.width(), video_profile.height()}].push_back(fps);
                        rgb_idx = i;
                    } else if (type == RS2_STREAM_DEPTH && video_profile.format() == depth_fmt) {
                        depth_res_n_fps[{video_profile.width(), video_profile.height()}].push_back(fps);
                        dpth_idx = i;
                    }
                }
                // Motion sensors: accelerometer and gyroscope
                else if (rs2::motion_stream_profile motion_profile = profile.as<rs2::motion_stream_profile>()) {
                    int freq = motion_profile.fps();
                    if (motion_profile.stream_type() == RS2_STREAM_ACCEL) {
                        accel_n_gyro.first.push_back(freq);
                    } else if (motion_profile.stream_type() == RS2_STREAM_GYRO) {
                        accel_n_gyro.second.push_back(freq);
                    }
                }
            }
        }

        // Check that something was found
        if (rgb_res_n_fps.empty() && depth_res_n_fps.empty() &&
            accel_n_gyro.first.empty() && accel_n_gyro.second.empty()) {
            std::cerr << "Warning: No supported RGB/Depth/Motion profiles found for the given device in retrieve_res_and_rate_combos().\n";
            return false;
        }

        return true;
    }
    catch (const rs2::error& e) {
        std::cerr << "RealSense error in retrieve_res_and_rate_combos(): "
                  << e.what() << " (" << e.get_failed_function() << ")\n";
        return false;
    }
}

void create_dir_if_not_exists(std::string directory) {
    const char* path = directory.c_str();
    boost::filesystem::path dir(path);
    if(boost::filesystem::create_directory(dir))
    {
        std::cerr<< "Directory Created: "<< directory <<std::endl;
    }
}

std::string get_sensor_name(const rs2::sensor& sensor)
{
    // Sensors support additional information, such as a human readable name
    if (sensor.supports(RS2_CAMERA_INFO_NAME))
        return sensor.get_info(RS2_CAMERA_INFO_NAME);
    else
        return "Unknown Sensor";
}

float get_depth_scale(rs2::device dev)
{
    // Go over the device's sensors
    for (rs2::sensor& sensor : dev.query_sensors())
    {
        // Check if the sensor if a depth sensor
        if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>())
        {
            return dpt.get_depth_scale();
        }
    }
    throw std::runtime_error("Device does not have a depth sensor");
}

bool check_imu_is_supported()
{
    bool found_gyro = false;
    bool found_accel = false;
    rs2::context ctx;
    for (auto dev : ctx.query_devices())
    {
        found_gyro = false;
        found_accel = false;
        for (auto sensor : dev.query_sensors())
        {
            for (auto profile : sensor.get_stream_profiles())
            {
                if (profile.stream_type() == RS2_STREAM_GYRO)
                    found_gyro = true;

                if (profile.stream_type() == RS2_STREAM_ACCEL)
                    found_accel = true;
            }
        }
        if (found_gyro && found_accel)
            break;
    }
    return found_gyro && found_accel;
}
