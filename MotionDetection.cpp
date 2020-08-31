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
        // get average brightness

        long totalValue = 0;

        // calculate total value of all pixels (GRAY)
        for (int i = 0; i < incoming_frame.rows; i++)
        {
            for (int j = 0; j < incoming_frame.cols; j++)
            {
                totalValue += incoming_frame.at<uchar>(i, j);
                // greyImage.at<uchar>(i, j) = (greyImage.at<uchar>(i, j)) * ;
            }
        }

        // calculate average light value
        int averageLightValue = totalValue / incoming_frame.total();

        // std::cout << "light: " << averageLightValue << std::endl;

        // check against base frame for disparities
        // find a way to get a baseline...

        cv::Mat motion_frame(incoming_frame.rows, incoming_frame.cols, cv::IMREAD_ANYCOLOR);

        // this->captureFrame = incoming_frame;

        cv::absdiff(incoming_frame, this->baseFrame, motion_frame);
        this->setBase(incoming_frame);

        cv::cvtColor(motion_frame, motion_frame, cv::COLOR_BGR2GRAY);
        cv::blur(motion_frame, motion_frame, cv::Size(3, 3));
        cv::threshold(motion_frame, motion_frame, (averageLightValue - 40), (((averageLightValue + 100) > 255) ? 255 : averageLightValue + 100), cv::THRESH_BINARY);
        // cv::Canny(motion_frame, motion_frame, 0, 255);

        // std::vector<std::vector<cv::Point>> contours;
        // std::vector<cv::Vec4i> hierarchy;
        // Standard Hough Line Transform
        // std::vector<cv::Vec2f> lines;

        // cv::HoughLines(
        //     motion_frame,
        //     lines,
        //     1,
        //     CV_PI / 180,
        //     50,
        //     50,
        //     10);

        // for (int i = 0; i < lines.size(); i++)
        // {
        //     float rho = lines[i][0], theta = lines[i][1];
        //     cv::Point pt1, pt2;
        //     double a = cos(theta), b = sin(theta);
        //     double x0 = a * rho, y0 = b * rho;
        //     pt1.x = cvRound(x0 + 1000 * (-b));
        //     pt1.y = cvRound(y0 + 1000 * (a));
        //     pt2.x = cvRound(x0 - 1000 * (-b));
        //     pt2.y = cvRound(y0 - 1000 * (a));
        //     cv::line(incoming_frame, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
        // }

        // cv::findContours(
        //     motion_frame,
        //     contours,
        //     hierarchy,
        //     cv::RETR_TREE,
        // 	cv::CHAIN_APPROX_SIMPLE
        // );

        // for (int index = 0; index < contours.size(); index++)
        // {
        //     cv::RotatedRect rect = cv::minAreaRect(contours[index]);
        //     rect.points(this->points);
        // }

        // for (int i = 0; i < 4; i++)
        // {
        //     cv::line(
        //         incoming_frame,
        //         this->points[i],
        //         this->points[(i + 1) % 4],
        //         cv::Scalar(0, 0, 255),
        //         2,
        //         cv::LINE_AA);
        // }

        cv::imshow("motion_window", motion_frame);
        cv::waitKey(1);

        cv::imshow("livefeed", incoming_frame);
        cv::waitKey(1);

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

    void MotionDetection::printDetection(cv::Mat &incoming_image)
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