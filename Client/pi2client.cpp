#include "pi2client.h"
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include "type.h"
#include <QThread>

PI2Client::PI2Client(QObject *parent) : QObject(parent)
{
    this->mainwindow = new MainWindow();
    this->dialog     = new Dialog();
    //this->windowSize = QApplication::desktop()->screenGeometry();

    this->trayIcon = new QSystemTrayIcon();

    QAction* quitAction = new QAction("Quit", this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(onQuitClient()));

    QAction* showMainWindow = new QAction("Webcam Live", this);
    connect(showMainWindow, SIGNAL(triggered()), this, SLOT(onShowMainWindow()));

    QAction* showDetection = new QAction("Detection",this);
    connect(showDetection,SIGNAL(triggered()),this,SLOT(onShowDialog()));



    QMenu* systemTrayMenu = new QMenu("tray menu");
    systemTrayMenu->addAction(showMainWindow);
    systemTrayMenu->addAction(showDetection);
    systemTrayMenu->addAction(quitAction);




    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setToolTip("tray app");
    this->trayIcon->setContextMenu(systemTrayMenu);
    this->trayIcon->setIcon(QIcon(":/image/webcam_icon.jpg"));
    this->trayIcon->show();

}

PI2Client::~PI2Client()
{
    delete this->mainwindow;
    delete this->dialog;
}

void PI2Client::startClient()
{

    this->mainwindow->show();

    this->connection  = new Connection();
    this->connection->connectToServer();

    QObject::connect(
                this->connection,
                SIGNAL(newFrame(int,cv::Mat&)),
                this,
                SLOT(getNewFrameFromServer(int,cv::Mat&)),
                Qt::DirectConnection
                );
    QObject::connect(
                this,
                SIGNAL(newFrameFrimServer(int,cv::Mat&)),
                this->mainwindow,
                SLOT(getNewFrameFromServer(int,cv::Mat&)),
                Qt::DirectConnection
                );

    QObject::connect(
                this,
                SIGNAL(newFrameFrimServer(int,cv::Mat&)),
                this->dialog,
                SLOT(getNewFrameFromServer(int,cv::Mat&)),
                Qt::DirectConnection
                );

    QThread *t=new QThread();
    t->start();


}

void PI2Client::onQuitClient()
{
    emit closeClient();
}

void PI2Client::onShowMainWindow()
{

    if (this->mainwindow->isHidden()) {
        this->mainwindow->show();
    }
}

void PI2Client::onShowDialog()
{
    if (this->dialog->isHidden()) {
        this->dialog->show();
    }
}

void PI2Client::setWindowSize(QRect &rect)
{
    this->windowSize = rect;
}

void PI2Client::getNewFrameFromServer(int type, cv::Mat &image)
{
    if (this->mainwindow->isHidden()) return;

    switch (type) {
    case (int)ServerSendCommand::SEND_IMAGE_ORIGINAL:
        emit newFrameFrimServer(type,image);
        break;
    case (int)ServerSendCommand::SEND_IMAGE_MOTION:
        emit newFrameFrimServer(type,image);
        break;
    case (int)ServerSendCommand::SEND_IMAGE_DETECTION_FACE: 
        //this->dialog->move(windowSize.width(),0);
        this->dialog->show();
        emit newFrameFrimServer(type,image);
        break;
    default:
        break;
    }
}
