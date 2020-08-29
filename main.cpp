// main.cpp

#include <iostream>
#include <ctime>
#include <cstring>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/video.hpp>

#include "SecurityCamera.h"

int main()
{
    // opening new video capture device
    cv::VideoCapture capture(0);

    // video file
    cv::VideoWriter video;

    // if no errors in capture device
    if (capture.isOpened())
    {
        // get todays date
        time_t now = time(0);
        time_t end_time = now();

        // writes video to file labeled with current date at 60fps
        video.open(
            std::strcat(ctime(&now), ".mp4"),
            cv::VideoWriter::fourcc('M', 'P', '4', 'V'),
            60,
            cv::Size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT)));

        // infinite loop
        for (int index = 0; index < 360; index++)
        {
            cv::Mat frame;

            // each frame of the video is stored here
            capture >> frame;

            video.write(frame);
        }

        // cv::imwrite("output.jpg", frame);
    }
    else
    {
        std::cout << "Unable to open video stream..." << std::endl;
        return -1;
    }

    // gracefully closing the capture device
    capture.release();
    video.release();

    return 0;
}