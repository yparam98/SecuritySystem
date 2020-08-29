#include <iostream>
#include <chrono>
#include <ctime>

#include "SecurityCamera.h"

namespace system
{
    SecurityCamera::SecurityCamera()
    {
        this->height = 0;
        this->width = 0;
    }

    SecurityCamera::~SecurityCamera()
    {
    }

    SecurityCamera::SecurityCamera(int incoming_capture_stream, int incoming_duration)
    {
        this->capture_stream.open(incoming_capture_stream);

        this->height = capture_stream.get(cv::CAP_PROP_FRAME_HEIGHT);
        this->width = capture_stream.get(cv::CAP_PROP_FRAME_WIDTH);

        this->frame(cv::Size(height, width), cv::IMREAD_ANYCOLOR);

        this->duration = incoming_duration;

        this->setname();
    }

    void SecurityCamera::setname()
    {
        this->file_name.clear();
        this->file_name.assign(time(0));
        this->file_name.append(".mp4");
    }

    void SecurityCamera::run()
    {
        // start time
        auto now = std::chrono::steady_clock::now();

        // while end time is less than or equal to start time plus duration
        while ((std::chrono::steady_clock::now() - now).count() <= (duration * 60))
        {
        }
    }
} // namespace system