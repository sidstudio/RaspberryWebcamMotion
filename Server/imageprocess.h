#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QByteArray>
#include <opencv2/opencv.hpp>
#include <vector>

namespace utils {

    /**
     * @brief The ImageProcess class
     * @author L.Skrypka sidstudio.com.ua
     */
    class ImageProcess
    {
    public:
        ImageProcess();

        /*  convert data from cv::Mat to QByteArray */
        static QByteArray mat2ByteArray(const cv::Mat &image);

        /* convert data from QByteArray to cv::Mat */
        static cv::Mat byteArray2Mat(const QByteArray &byteArray);

        static std::vector<char> byteArray2StdVector( QByteArray &data);

        static QByteArray stdVector2ByteArray(const std::vector<unsigned char> &data);

        /* compress image */
        static QByteArray compressImage(cv::Mat& image);
    };

}


#endif // IMAGEPROCESS_H
