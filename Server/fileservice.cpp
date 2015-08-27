#include "fileservice.h"
#include <QDir>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>

FileService::FileService(QObject *parent) : QObject(parent)
{
    path = "./photo/";
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }
}

void FileService::getNewFrame(server::ImageData data)
{

    qDebug()<<"FileService::getNewFrame();";

    QString _path;
    _path.append(path);
    _path.append(data.time.toString());
    _path.append(".jpg");

    cv::imwrite(_path.toStdString().c_str(),data.image);
}
