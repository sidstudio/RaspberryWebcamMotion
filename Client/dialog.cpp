#include "dialog.h"
#include "ui_dialog.h"
#include <QGraphicsEffect>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include "type.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->curentImagePos = 0;
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->hide();
}


void Dialog::getNewFrameFromServer(int type, cv::Mat& image)
{
    QImage _qimage((uchar*)image.data,image.cols,image.rows,image.step,QImage::Format_RGB888);

    switch (type) {
    case (int)ServerSendCommand::SEND_IMAGE_DETECTION_FACE:
        this->listImage.append(image);
        this->curentImagePos = listImage.size();
        ui->labelImage->setPixmap(QPixmap::fromImage(_qimage));
        break;
    default:
        break;
    }

}


void Dialog::on_pushButtonPrev_clicked()
{
    if (this->curentImagePos == 0) {
        return;
    }

    cv::Mat image = this->listImage.at(this->curentImagePos-1);
    this->curentImagePos--;

    QImage _qimage((uchar*)image.data,image.cols,image.rows,image.step,QImage::Format_RGB888);
    ui->labelImage->setPixmap(QPixmap::fromImage(_qimage));

}

void Dialog::on_pushButton_2_clicked()
{
    if (this->curentImagePos == this->listImage.size()) {
        return;
    }


    cv::Mat image = this->listImage.at(this->curentImagePos);
    this->curentImagePos++;

    QImage _qimage((uchar*)image.data,image.cols,image.rows,image.step,QImage::Format_RGB888);
    ui->labelImage->setPixmap(QPixmap::fromImage(_qimage));


}

void Dialog::on_pushButton_3_clicked()
{
    this->curentImagePos = 0;
    this->listImage.clear();
}
