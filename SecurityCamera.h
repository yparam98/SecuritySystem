#ifndef YPARAM_SECURITYCAMERA_H
#define YPARAM_SECURITYCAMERA_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/video.hpp>
#include <opencv4/opencv2/imgproc.hpp>

namespace yparam
{
    const int FPS{60};

    class SecurityCamera
    {
    private:
        cv::VideoCapture capture_stream;
        cv::VideoWriter video_file;
        cv::Mat frame;

        std::string file_name{""};

        int capture_width{0};
        int capture_height{0};

        int duration_minutes{0};

        void setname();

    public:
        SecurityCamera();
        SecurityCamera(int, int = 60); // capture stream, duration
        ~SecurityCamera();

        void run();
    };
} // namespace yparam

#endif // YPARAM_SECURITYCAMERA_H