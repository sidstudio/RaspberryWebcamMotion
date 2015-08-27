#include "imageprocess.h"

#include <QDataStream>

namespace utils {

ImageProcess::ImageProcess()
{

}

/**
 * @brief ImageProcess::byteArray2Mat
 * @param byteArray
 * @return Images converted from a sequence of bytes in a format cv::Mat
 */
cv::Mat ImageProcess::byteArray2Mat(const QByteArray &byteArray)
{
    QDataStream stream(byteArray);
    int matType, rows, cols;
    QByteArray data;

    /* Installation of the reading of data from an array of bytes */
    stream >> matType;
    stream >> rows;
    stream >> cols;
    stream >> data;

    /* create image */
    cv::Mat mat( rows, cols, matType, (void*)data.data() );

    return mat.clone();
}

/**
 * @brief ImageProcess::mat2ByteArray
 * @param image
 * @return byte, converted from format cv::Mat
 *
 */
QByteArray ImageProcess::mat2ByteArray(const cv::Mat &image)
{
    QByteArray byteArray;
    QDataStream stream( &byteArray, QIODevice::WriteOnly );

    /* setting the order of the bytes in the array */
    stream << image.type();
    stream << image.rows;
    stream << image.cols;

    const size_t data_size = image.cols * image.rows * image.elemSize();
    QByteArray data = QByteArray::fromRawData( (const char*)image.ptr(), data_size );

    /* setup of the image data */
    stream << data;

    return byteArray;
}

QByteArray ImageProcess::stdVector2ByteArray(const std::vector<unsigned char> &data)
{
    QByteArray _tmp;// = QByteArray::fromRawData(data.data(),data.size());//(data.data(),data.size());

    // TODO: change copy
    for (int var = 0; var < data.size(); ++var) {
        _tmp.append(data.at(var));
    }

    return _tmp;
}


std::vector<char> ImageProcess::byteArray2StdVector( QByteArray &data)
{
    return std::vector<char>(data.begin(), data.end());
}

QByteArray ImageProcess::compressImage(cv::Mat &image)
{
    /* buffer for compress */
    std::vector<unsigned char> buff;
    /* parameters for compress */
    std::vector<int> param = std::vector<int>(2);
    param[0]=CV_IMWRITE_JPEG_QUALITY;
    param[1]=95;//default(95) 0-100

    /*
     * Encode the image to transfer
     * Variable buff is now compressed images to be sent over the network
     */
    cv::imencode(".jpg",image,buff,param);

    QByteArray imageToEmit = utils::ImageProcess::stdVector2ByteArray(buff);
    return imageToEmit;
}


}



