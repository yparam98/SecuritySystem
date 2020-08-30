#include <iostream>
#include "MotionDetection.h"

namespace yparam
{
    MotionDetection::MotionDetection()
    {
        this->catchType = "";
        this->detectionTime = time(0);
        this->detected = false;
        this->faceDetected = false;

        cv::namedWindow("motion_window", cv::WINDOW_AUTOSIZE);
    }

    MotionDetection::~MotionDetection()
    {
    }

    bool MotionDetection::checkDetection(cv::Mat incoming_frame)
    {
        // check against base frame for disparities
        cv::Mat difference_arr(incoming_frame.rows, incoming_frame.cols, cv::IMREAD_ANYCOLOR);

        this->captureFrame = incoming_frame;
        this->previousFrame = this->baseFrame;
        cv::absdiff(captureFrame, previousFrame, difference_arr);
        
        cv::imshow("frame", difference_arr);

        cv::waitKey(1000);

        return true;
    }

    time_t MotionDetection::getCaptureTime() const
    {
        return this->detectionTime;
    }

    void MotionDetection::setBase(cv::Mat incoming_base)
    {
        this->baseFrame = incoming_base;
    }

    void MotionDetection::printDetection(cv::Mat incoming_image)
    {
        // print a rectangle showing object of motion detection on the incoming image
    }

    cv::VideoCapture operator>>(cv::VideoCapture incoming_capture_stream, MotionDetection& incoming_obj)
    {
        cv::Mat frame;
        incoming_capture_stream >> frame;
        incoming_obj.setBase(frame);

        return incoming_capture_stream;
    }
} // namespace yparam