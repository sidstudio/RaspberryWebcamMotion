#include "pooldetections.h"
#include <QDebug>
#include <QFile>
#include <imageprocess.h>
#include <QDataStream>
#include <type.h>


PoolDetections::PoolDetections(QObject *parent) : QObject(parent)
{

    QFile file("haarcascade_frontalface_alt2.xml");
    if (!file.exists()) {
        qDebug()<<"cant find file";
    } else {
        this->faseCascade.load("haarcascade_frontalface_alt2.xml");
    }
}

void PoolDetections::startRecognitions()
{
    qDebug()<<"start regognitions";
    while (true) {

        server::ImageData data;
        this->_mutex.lock();
        int count = this->imageList.size();
        if ( count > 0) {
            data = this->imageList.dequeue();
            this->_mutex.unlock();

            cv::Mat gray;

            /* convert images to gray */
            cv::cvtColor(data.image,gray,CV_RGB2GRAY);

            /* search for persons in the image */
            std::vector<cv::Rect> faces;
            this->faseCascade.detectMultiScale( gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

            /* drawing a circle around the face */
            for( int i = 0; i < faces.size(); i++ ){
                cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                qDebug()<<"found faces in image";
                cv::ellipse( data.image, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0 );
            }

            if (faces.size() > 0) {
                cv::putText(data.image,data.time.toString().toStdString(),cv::Point(30,30),cv::FONT_HERSHEY_COMPLEX_SMALL, 1,cv::Scalar(0,255,255));
                emit sendDetectionFrame(data);
                sendFrame(data.image);
            }

            qDebug()<<count<<data.time;
        } else {
            this->_mutex.unlock();
        }

    }
}

void PoolDetections::sendFrame(cv::Mat &image)
{

    std::vector<unsigned char> buff;
    /* параметры для кодирования */
    std::vector<int> param = std::vector<int>(2);
    param[0]=CV_IMWRITE_JPEG_QUALITY;
    param[1]=95;//default(95) 0-100

    cv::imencode(".jpg",image,buff,param);

    QByteArray imageToEmit = utils::ImageProcess::stdVector2ByteArray(buff);
    emit newFrame((int)server::ServerSendCommand::SEND_IMAGE_DETECTION_FACE,imageToEmit);
}





void PoolDetections::imageToDetections(server::ImageData data)
{
    qDebug()<<"PoolDetections::imageToDetections();<<get new image"<<data.time;
    this->_mutex.lock();
    this->imageList.enqueue(data);
    this->_mutex.unlock();
}
