#ifndef POOLDETECTIONS_H
#define POOLDETECTIONS_H

#include <QObject>
#include <QQueue>
#include <opencv2/opencv.hpp>
#include <type.h>
#include <QMutex>

/**
 * @brief The PoolDetections class
 * @author L.Skrypka sidstudio.com.ua
 *
 */
class PoolDetections : public QObject
{
    Q_OBJECT
public:
    explicit PoolDetections(QObject *parent = 0);

private:
    /* queue image for detections faces */
    QQueue<server::ImageData> imageList;

    QMutex _mutex;
    /* Haar cascade for detrections */
    cv::CascadeClassifier faseCascade;

    void sendFrame(cv::Mat& image);
signals:

    void newFrame(int command,const QByteArray& data);
    void sendDetectionFrame(server::ImageData data);

public slots:
    void startRecognitions();
    void imageToDetections(server::ImageData data);
};

#endif // POOLDETECTIONS_H
