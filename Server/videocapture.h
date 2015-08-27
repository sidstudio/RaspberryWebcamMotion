#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H


#include <QByteArray>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <QDebug>
#include <X11/Xlib.h>
#include "type.h"
#include <QObject>
#include "type.h"

namespace camera {


    /**
     * @brief The VideoCapture class
     * @author L.Skrypka sidstudio.com.ua
     *
     * capture image from camera
     *
     */
    class VideoCapture : public QObject
    {
        Q_OBJECT
    public:
        explicit VideoCapture(QObject *parent = 0);


    private:

        /* the minimum area of the circuit for filtering small changes */
        static constexpr double MIN_CONTOUR_AREA = 200.0;

        /* camera Resolution */
        int imageCaptureWidth;
        int imageCaptureHeight;



        /* a flag that controls the camera capture */
        bool isCapture;

        cv::VideoCapture *videoCapture;

        void sendImageToEmit(cv::Mat &image);

    signals:
        /* a signal that indicates that he came from a new frame capture camera */
        void newRawFrame(int command, const QByteArray& frame);

        void newDetectionFrame(int command, const QByteArray& frame);

        /* a signal that is generated when an error */
        void error(camera::VideoCaptureError error);

        void finish();

        void frameToDetections(server::ImageData data);

    public slots:

        /* a slot that runs the capture camera */
        void startCapture();

        /* slot that stops the capture camera */
        void stopCapture();
    };

}



#endif // VIDEOCAPTURE_H
