#ifndef YPARAM_MOTIONDETECTION_H
#define YPARAM_MOTIONDETECTION_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/video.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>

namespace yparam
{
    class MotionDetection
    {
    private:
        bool detected{false};
        time_t detectionTime;
        cv::Mat previousFrame;
        cv::Mat captureFrame;
        cv::Mat baseFrame;
        std::string catchType{""};
        bool faceDetected{false};        
        cv::Point2f points[4];

    public:
        MotionDetection();
        ~MotionDetection();

        bool checkDetection(cv::Mat);
        time_t getCaptureTime() const;
        void setBase(cv::Mat);
        void printDetection(cv::Mat&);

        // friend void operator>>(cv::VideoCapture, MotionDetection);
    };
    cv::VideoCapture operator>>(cv::VideoCapture, MotionDetection&);
} // namespace yparam

#endif // YPARAM_MOTIONDETECTION_H