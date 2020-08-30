#include "SecurityCamera.h"

namespace yparam
{
    SecurityCamera::SecurityCamera()
    {
        this->capture_height = 0;
        this->capture_width = 0;
    }

    SecurityCamera::~SecurityCamera()
    {
        this->capture_stream.release();
        this->video_file.release();
    }

    SecurityCamera::SecurityCamera(int incoming_capture_stream, int incoming_duration)
    {
        this->capture_stream.open(incoming_capture_stream);

        this->capture_height = capture_stream.get(cv::CAP_PROP_FRAME_HEIGHT);
        this->capture_width = capture_stream.get(cv::CAP_PROP_FRAME_WIDTH);

        this->frame = cv::Mat(cv::Size(this->capture_width, this->capture_height), cv::IMREAD_ANYCOLOR);

        this->duration_minutes = incoming_duration;

        this->setname();

        this->video_file.open(
            this->file_name,
            cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
            FPS,
            cv::Size(this->capture_width, this->capture_height));
    }

    void SecurityCamera::setname()
    {        
        time_t timer;
        time(&timer);

        this->file_name.clear();
        this->file_name.assign(ctime(&timer));
        this->file_name.append(".mp4");
    }

    void SecurityCamera::run()
    {
        // right now, the program only runs for the duration specified... which is good... but the actual
        // video run time is much shorter than that. I believe the issue is caused by the FPS... so I need
        // to find a way to calculate the total run time while taking into account the FPS and the duration
        // specified.

        // FIXED = VIDEO DURATION IS NOW CORRECT

        /*
        
        currently the FPS is set at 60... that means 60 frames per second... if duration is one minute, then
        that will be 60 seconds... total frames will be 60FPS x 60 seconds = 3600 frames in total...

        */

        // start time
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> diff = std::chrono::steady_clock::now() - now;

        // this loop runs until duration elapses
        // just because it runs for 60 seconds doesn't mean that 60 frames will be assigned to the video...
        for (int index = 0; index < (this->duration_minutes * 60 * 60); index++) // minutes x seconds x frames = 1 x 60 x 60 = 3600 frames = 1 minute of video
        {
            this->capture_stream >> this->frame; // perform operations on this frame
            this->video_file.write(this->frame);

            // keep time watch at end
            diff = std::chrono::steady_clock::now() - now;
            std::cout << diff.count() << std::endl;
        }

        /*

        TIMES:

        3 MIN = 2:15
        1 MIN = 0:45

        */
    }
} // namespace yparam