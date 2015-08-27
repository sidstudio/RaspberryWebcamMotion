#include "videocapture.h"
#include <QByteArray>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <QDebug>
#include <X11/Xlib.h>
#include "type.h"
#include "imageprocess.h"
#include <vector>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QTime>

namespace camera {

    /**
     * @brief VideoCapture::VideoCapture
     * @param parent
     *
     *  initializes the parameters for the camera
     */
    VideoCapture::VideoCapture(QObject *parent) : QObject(parent)
    {
        /* set the default resolution of the camera  640х480 */
        this->imageCaptureHeight    = 480;//144;//480;
        this->imageCaptureWidth     = 640;//176;//640;

        this->isCapture = false;

        /*
         * if you want show diplay image in gui on raspberry you need
         * call  XInitThreads() otherwise we get an error
         *
         *   [xcb] Unknown request in queue while dequeuing
         *   [xcb] Most likely this is a multi-threaded client and XInitThreads has not been called
         *   [xcb] Aborting, sorry about that.
         */
        XInitThreads();
    }

    /**
     * @brief VideoCapture::startCapture
     *
     * Slot that runs the capture camera, the default is
     * any available camera. Interrupt capture camera signal is produced,
     *  which is processed slot stopCapture().
     *
     */
    void VideoCapture::startCapture()
    {

        qDebug()<<"startCapture();";

        /*
         *
         * 0 - default
         */
        int cameraId = 0;

        this->videoCapture = new cv::VideoCapture(cameraId);

        if (!this->videoCapture->isOpened()) {
            qDebug()<<"cant open camera: "<<cameraId;
            /*
             * If you get an error opening the send
             * Error signal and closure trapping camera
             */
            emit error(camera::VideoCaptureError::OPENCV_CAMERA_NOT_OPEN);
            emit finish();
            return;
        }

        /* setup resolutions for camera */
        this->videoCapture->set(CV_CAP_PROP_FRAME_WIDTH,this->imageCaptureWidth);
        this->videoCapture->set(CV_CAP_PROP_FRAME_HEIGHT,this->imageCaptureHeight);

        qDebug()<<"camera property: "<<this->videoCapture->get(CV_CAP_PROP_FRAME_WIDTH)<<" "<<this->videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT);


        cv::Mat frame;


        this->isCapture = true;

        /* a counter for transmitting frames over the network, not all of the frames, and each FRAME_OFFSET */
        int i = 0;
        int FRAME_OFFSET = 3;

        /* a list of paths that were found in motion detection */
        std::vector<std::vector<cv::Point> > contours;


        // Take images and convert them to gray
        cv::Mat result;

        cv::Mat prev_frame;
        (*videoCapture)>>result;
        prev_frame = result.clone();

        cv::Mat current_frame;
        (*videoCapture)>>current_frame;

        cv::Mat next_frame;
        (*videoCapture)>>next_frame;


        cv::cvtColor(current_frame, current_frame, CV_RGB2GRAY);
        cv::cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);
        cv::cvtColor(next_frame, next_frame, CV_RGB2GRAY);


        // d1 and d2 for calculating the differences
        // result, the result of and operation, calculated on d1 and d2
        // number_of_changes, the amount of changes in the result matrix.
        // color, the color for drawing the rectangle when something has changed.
        cv::Mat d1, d2, motion;

        // Erode kernel
        cv::Mat kernel_ero = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));

        int currentFrame = 0;
        int FRAME_OFFSET_DETECTION = 6;


        while (true) {

            prev_frame.release();
            prev_frame = current_frame;
            current_frame.release();
            current_frame = next_frame;

            next_frame.release();
            (*videoCapture)>>next_frame;

            //qDebug()<<"get Frame time"<<QTime::currentTime();

            frame.release();
            frame = next_frame.clone();

            result.release();
            result = next_frame;

            currentFrame++;

            cv::cvtColor(next_frame, next_frame, CV_RGB2GRAY);

            cv::absdiff(prev_frame, next_frame, d1);
            cv::absdiff(next_frame, current_frame, d2);
            cv::bitwise_and(d1, d2, motion);
            cv::threshold(motion, motion, 35, 255, CV_THRESH_BINARY);
            cv::erode(motion, motion, kernel_ero);

            if (currentFrame == FRAME_OFFSET_DETECTION) {

                //qDebug()<<"frame offset:"<<QTime::currentTime();

                contours.clear();
                cv::findContours(motion,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

                /*
                 * Flag which indicates that the image was changing,
                 * Which exceeds the threshold area
                 */
                bool isDetection = false;
                //cv::Mat drawImage = frame.clone();

                /* We go through all the found contours and filter excess */
                for (int i=0; i<contours.size(); i++) {
                    /* We calculate the area of the contour */
                    double area = cv::contourArea(contours[i]);
                    /* if the area is less than the minimum circuit then go to the next contour found*/
                    if (area < MIN_CONTOUR_AREA) {
                        continue;
                    }
                    /* We define a rectangle which includes a circuit */
                    cv::Rect rect = cv::boundingRect(contours[i]);

                    isDetection = true;
                    //if (rect.width<50 && rect.height<50) continue;
                    /* рисуем прямоуголник на изображении */
                    //cv::rectangle(drawImage,rect,cv::Scalar(0,0,255));
                    //cv::drawContours(frame,contours,i,cv::Scalar(0,0,255),2);

                }

                if (isDetection) {
                    qDebug()<<"Found motion on the picture";


                    //QByteArray b = utils::ImageProcess::compressImage(drawImage);
                    //emit newDetectionFrame((int)server::ServerSendCommand::SEND_IMAGE_MOTION,b);
                    sendImageToEmit(frame);
                }

                currentFrame = 0;
            }

            /*
             * Interrupt reading camera and sends a signal
             * Complete if isCapture = false
             */
            if (!this->isCapture) {
                /* release camera */
                this->videoCapture->release();
                delete this->videoCapture;
               // cv::destroyWindow("image");

                /* send signal finish*/
                emit finish();
                break;
            }


            i++;
            if (i==FRAME_OFFSET) {
                i=0;
                QByteArray ba = utils::ImageProcess::compressImage(frame);
                emit newRawFrame((int)server::ServerSendCommand::SEND_IMAGE_ORIGINAL,ba);
                //sendImageToEmit(frame);

            }

        }

    }

    void VideoCapture::sendImageToEmit(cv::Mat &image)
    {

        server::ImageData data;
        data.time = QTime::currentTime();
        data.image = image;

        qDebug()<<"sendIMageToEmit:"<<data.time;
        emit frameToDetections(data);
    }


    /**
     * @brief VideoCapture::stopCapture
     *
     * slot which stops the capture with the camera
     */
    void VideoCapture::stopCapture()
    {
        qDebug()<<"stopCapture();";
        this->isCapture = false;
    }

}
