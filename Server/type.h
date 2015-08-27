#ifndef TYPE
#define TYPE

#include <QTime>
#include <opencv2/opencv.hpp>

namespace camera {
    /**
     * @brief The VideoCaptureError enum
     * @author L.Skrypka sidstudio.com.ua
     *
     * Class determines the type of error that occurred when the server work
     */
    enum class VideoCaptureError:int
    {
        OPENCV_CAMERA_NOT_OPEN  = 1
    };
}

namespace server {

    /**
     * @brief The ServerSignal enum
     * L.Skrypka sidstudio.com.ua
     *
     * Class determines the type of signal occurred on the server
     */
    enum class ServerSignal: int
    {
        SEND_STOP_CAPTURE   = 1,
        SEND_START_CAPTURE  = 2,
        SERVER_START_ERROR  = 3
    };

    enum class ServerSendCommand: int
    {
        SEND_IMAGE_ORIGINAL        = 1,
        SEND_IMAGE_DETECTION_FACE  = 2,
        SEND_IMAGE_MOTION          = 3
    };

    struct ImageData {
        QTime time;
        cv::Mat image;
    };
}



/**
 *  Переопридиления оператора << для std::cout
 
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}
*/


#endif // TYPE

