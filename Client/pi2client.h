#ifndef PI2CLIENT_H
#define PI2CLIENT_H

#include <QObject>
#include <mainwindow.h>
#include <dialog.h>
#include <opencv2/opencv.hpp>
#include <QRect>
#include <QSystemTrayIcon>
#include "connection.h"

class PI2Client : public QObject
{
    Q_OBJECT
public:
    explicit PI2Client(QObject *parent = 0);
    ~PI2Client();
    void startClient();
    void setWindowSize(QRect& rect);
private:

    QRect windowSize;

    Dialog* dialog;

    QSystemTrayIcon *trayIcon;

    MainWindow* mainwindow;

    Connection* connection;

signals:
    void closeClient();
    void newFrameFrimServer(int type, cv::Mat& image);
public slots:
    void onQuitClient();
    void onShowMainWindow();
    void onShowDialog();
    void getNewFrameFromServer(int type, cv::Mat& image);
};

#endif // PI2CLIENT_H
