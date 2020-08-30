#include <iostream>
#include <vector>

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
        cv::namedWindow("livefeed", cv::WINDOW_AUTOSIZE);
    }

    MotionDetection::~MotionDetection()
    {
    }

    bool MotionDetection::checkDetection(cv::Mat incoming_frame)
    {
        // check against base frame for disparities
        // find a way to get a baseline...

        cv::Mat motion_frame(incoming_frame.rows, incoming_frame.cols, cv::IMREAD_ANYCOLOR);

        // this->captureFrame = incoming_frame;

        cv::absdiff(incoming_frame, this->baseFrame, motion_frame);
        // this->baseFrame = incoming_frame;

        cv::cvtColor(motion_frame, motion_frame, cv::COLOR_BGR2GRAY);
        cv::blur(motion_frame, motion_frame, cv::Size(3, 3));
        cv::threshold(motion_frame, motion_frame, 50, 255, cv::THRESH_BINARY);
        cv::Canny(motion_frame, motion_frame, 0, 255);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(
            motion_frame,
            contours,
            hierarchy,
            cv::RETR_TREE,
			cv::CHAIN_APPROX_SIMPLE
        );

        for (int index = 0; index < contours.size(); index++)
        {
            cv::RotatedRect rect = cv::minAreaRect(contours[index]);
            rect.points(this->points);
        }

        // cv::imshow("frame", motion_frame);
        // cv::waitKey(1);
        // cv::imshow("livefeed", incoming_frame);
        // cv::waitKey(1);

        // this->baseFrame = incoming_frame;

        return true;
    }

    time_t MotionDetection::getCaptureTime() const
    {
        return this->detectionTime;
    }

    void MotionDetection::setBase(cv::Mat incoming_base)
    {
        cv::blur(incoming_base, this->baseFrame, cv::Size(3, 3));
    }

    void MotionDetection::printDetection(cv::Mat incoming_image)
    {
        // print a rectangle showing object of motion detection on the incoming image
        for (int i = 0; i < 4; i++)
        {
            cv::line(
                incoming_image,
                this->points[i],
                this->points[(i + 1) % 4],
                cv::Scalar(0, 255, 0),
                2,
                cv::LINE_AA);
        }
    }

    cv::VideoCapture operator>>(cv::VideoCapture incoming_capture_stream, MotionDetection &incoming_obj)
    {
        cv::Mat frame;
        incoming_capture_stream >> frame;
        incoming_obj.setBase(frame);

        return incoming_capture_stream;
    }
} // namespace yparam