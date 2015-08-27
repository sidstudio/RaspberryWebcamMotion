#include "connection.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <QDataStream>
#include <QList>
#include <QSslError>
#include <QDebug>
#include <QFile>
#include <QSslKey>

Connection::Connection(QObject *parent) : QObject(parent)
{

    this->_blockSize = 0;

    this->sslsocket = new QSslSocket(this);
    this->sslsocket->setProtocol(QSsl::TlsV1_2);

    connect(
            this->sslsocket,
            SIGNAL(readyRead()),
            this,
            SLOT(readyRead())
        );
    connect(
                this->sslsocket,
                SIGNAL(encrypted()),
                this,
                SLOT(encrypted())
            );

    connect(
                this->sslsocket,
                SIGNAL (disconnected()),
                this,
                SLOT(disconnected())
            );
    connect(
                this->sslsocket,
                SIGNAL(connected()),
                this,
                SLOT(connected())
            );
    connect(
                this->sslsocket,
                SIGNAL(sslErrors(const QList<QSslError> &)),
                this,
                SLOT(socketSslErrors(const QList<QSslError> &))
            );
    connect(
                this->sslsocket,
                SIGNAL(error(QAbstractSocket::SocketError)),
                this,
                SLOT(socketError(QAbstractSocket::SocketError))
            );

}

void Connection::connectToServer()
{
    this->sslsocket->connectToHostEncrypted("192.168.0.72",1258);


    this->sslsocket->addCaCertificates("key072.pem");

    this->_blockSize = 0;

    if (this->sslsocket->waitForEncrypted()) {
        qDebug()<<"connected";
        this->sslsocket->write("hello");
        if(this->sslsocket->waitForBytesWritten()) {
            qDebug()<<"send msg";
        } else {
            qDebug()<<this->sslsocket->errorString();
        }
    } else {
        qDebug()<<"error:"<<this->sslsocket->errorString();
    }

}




void Connection::readyRead()
{
    QDataStream in(this->sslsocket);
    //qDebug()<<"ready read"<<this->_blockSize ;

    if (_blockSize == 0) {
        if (sslsocket->bytesAvailable() < sizeof(_blockSize))
            return;
        in >> _blockSize;
        //qDebug()<<_blockSize;
    }

    quint64 bytesAvail;
    if ((bytesAvail = sslsocket->bytesAvailable()) < _blockSize) {
        return;
    }



    qint32 type;
    in >> type;
    QByteArray ba;
    in >> ba;

    //cv::Mat img = byteArrayToMat(ba);

    std::vector<char> buff = converByteToVector(ba);
    cv::Mat img = cv::imdecode(cv::Mat(buff),CV_LOAD_IMAGE_COLOR);


    if (type == 2) {
       // cv::imshow("img",img);
    } else if (type == 3) {
       // cv::imshow("detection",img);
    } else if (type == 1) {
        //cv::imshow("original",img);
    }

    emit newFrame(type,img);

    if (in.status() != QDataStream::Ok) {
    }

    _blockSize = 0;
}

std::vector<char> Connection::converByteToVector(const QByteArray &array)
{
    return std::vector<char>(array.begin(),array.end());
}

QByteArray Connection::converVectorToByte(const std::vector<char> &vector)
{
    return QByteArray(vector.data(),vector.size());
}

cv::Mat Connection::byteArrayToMat(const QByteArray &data)
{
    QDataStream stream(data);
    int matType,rows,cols;
    QByteArray dataIMg;

    stream >> matType;
    stream >> rows;
    stream >> cols;
    stream >> dataIMg;
    cv::Mat mat(rows,cols,matType,(void*)dataIMg.data());
    return mat.clone();
}

void Connection::connected()
{
    qDebug()<<"connected();";
}

void Connection::encrypted()
{
    qDebug()<<"encrypted();";
}

void Connection::disconnected()
{
    qDebug()<<"disconnected();";
}

void Connection::socketSslErrors(const QList<QSslError> &list)
{
    qDebug()<<"socketSslErrors();";
}

void Connection::socketError(QAbstractSocket::SocketError error)
{
    qDebug()<<"- socketError();";
    qDebug()<<"-"<<error;
}

bool Connection::sendData(const QByteArray data)
{
    if (!this->sslsocket) {
        qDebug()<<"ssl socket is null";
        return false;
    }

    this->sslsocket->write(data);
    this->sslsocket->waitForBytesWritten();

}


