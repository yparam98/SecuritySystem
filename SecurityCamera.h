#ifndef SYSTEM_SECURITYCAMERA_H
#define SYSTEM_SECURITYCAMERA_H

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/video.hpp>

#include <ctime>
#include <string>

namespace system
{
    const int FPS{60};

    class SecurityCamera
    {
    private:
        cv::VideoCapture capture_stream;
        cv::Mat frame;

        std::string file_name{""};

        int width{0};
        int height{0};

        int duration{0};

        void set_name(); // set the file name for the video file
        void set_base(); // create a "base" line image from the video (something to compare other frames to for motion capture)
        bool run_motion_detection(); // compare the frame with the base line to detect any changes 
        bool run_facial_detection(); // run facial detection on the frames with motion detected

    public:
        SecurityCamera();
        SecurityCamera(int, int = 60);
        ~SecurityCamera();

        void run();
    };
} // namespace system

#endif // SYSTEM_SECURITYCAMERA_H