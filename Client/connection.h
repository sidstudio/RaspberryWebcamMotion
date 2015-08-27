#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSslSocket>
#include <opencv2/opencv.hpp>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    void connectToServer();
    bool sendData(const QByteArray data);
private:

    quint64 _blockSize; //текущий размер блокаданных

    cv::Mat byteArrayToMat(const QByteArray &data);

    QByteArray converVectorToByte(const std::vector<char> &vector);
    std::vector<char> converByteToVector(const QByteArray &array);

    QSslSocket *sslsocket;
signals:
    void newFrame(int type,cv::Mat& image);

public slots:
    void readyRead();
    void encrypted();
    void connected();
    void disconnected();
    void socketError(QAbstractSocket::SocketError error);
    void socketSslErrors(const QList<QSslError> &list);
};

#endif // CONNECTION_H
