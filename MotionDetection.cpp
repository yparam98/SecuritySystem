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
        // cv::namedWindow("thresh", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("livefeed", cv::WINDOW_AUTOSIZE);
    }

    MotionDetection::~MotionDetection()
    {
    }

    bool MotionDetection::checkDetection(cv::Mat incoming_frame)
    {
        // this->calc_light(incoming_frame);

        cv::Mat motion_frame(incoming_frame.rows, incoming_frame.cols, cv::IMREAD_ANYCOLOR);
        cv::blur(incoming_frame, incoming_frame, cv::Size(5,5));
        cv::absdiff(incoming_frame, this->baseFrame, motion_frame); // finding binary differences

        this->setBase(incoming_frame);

        cv::Mat element = cv::getStructuringElement(cv::MORPH_OPEN, cv::Size(5,5));

        cv::erode(motion_frame, motion_frame, element); // eroding the differential
        cv::dilate(motion_frame, motion_frame, element);

        

        cv::cvtColor(motion_frame, motion_frame, cv::COLOR_BGR2GRAY);

        cv::blur(motion_frame, motion_frame, cv::Size(5,5)); // blurring the differential image // higher the brightness, lower the blur; lower the brightness, higher the blur

        cv::threshold(motion_frame, motion_frame, ((this->avg_light - 70) < 0 ? 0 : (this->avg_light - 70)), 255, cv::THRESH_BINARY); // doesn't detect it as a blob... more of a cluster of blobs...

        // cv::Canny(motion_frame, motion_frame, 0, 255);
        // do we need edge detection? the idea is to get a bigger blob to grab onto, so leaving it at threshold might be beneficial for this scenario

        // use kmeans to group each "cluster" as 1 object
        // then save those points encasing each cluster

        /*
        
        79   11   7
        89   17   5
        114  57   2

        */

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

    void MotionDetection::calc_light(cv::Mat img)
    {
        long totalValue = 0;

        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                totalValue += img.at<uchar>(i, j);
                // greyImage.at<uchar>(i, j) = (greyImage.at<uchar>(i, j)) * ;
            }
        }

        this->avg_light = totalValue / img.total();

        this->kernel = ((this->avg_light < 75) ? cv::Size(5, 5) : cv::Size(3, 3));
    }

    void MotionDetection::setBase(cv::Mat incoming_base)
    {
        cv::blur(incoming_base, this->baseFrame, this->kernel);
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
        incoming_obj.calc_light(frame);
        incoming_obj.setBase(frame);

        return incoming_capture_stream;
    }
} // namespace yparam