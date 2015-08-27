#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QImage>
#include <QIcon>
#include <QDebug>
#include <type.h>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getNewFrameFromServer(int type, cv::Mat& image)
{

    QImage _qimage((uchar*)image.data,image.cols,image.rows,image.step,QImage::Format_RGB888);


    switch (type) {
    case (int)ServerSendCommand::SEND_IMAGE_DETECTION_FACE:
        //ui->labelRecognition->setPixmap(QPixmap::fromImage(_qimage));
        break;
    case (int)ServerSendCommand::SEND_IMAGE_MOTION:
        //ui->labelMotion->setPixmap(QPixmap::fromImage(_qimage));
        break;
    case (int)ServerSendCommand::SEND_IMAGE_ORIGINAL:
        ui->labelMain->setPixmap(QPixmap::fromImage(_qimage));
        //ui->labelRaw->setPixmap(QPixmap::fromImage(_qimage));
    default:
        break;
    }


}
