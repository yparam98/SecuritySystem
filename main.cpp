// main.cpp

#include <iostream>
#include <ctime>
#include <cstring>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/video.hpp>

#include "SecurityCamera.h"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        yparam::SecurityCamera securitycamera(0, std::atoi(argv[1]));
        securitycamera.run();

        return 0;
    }
    else
    {
        std::cout << "Usage: ./main <duration_of_monitoring>" << std::endl;
        return -1;
    }
}