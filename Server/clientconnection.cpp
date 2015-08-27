#include "clientconnection.h"

#include <QSslConfiguration>
#include <QSslKey>
#include <QList>
#include <QSslError>
#include <QFile>
#include <QSslCertificate>
#include <QDataStream>
#include <QDebug>

ClientConnection::ClientConnection(QObject *parent)
{
    this->socketID = 0;
}

bool ClientConnection::setSocket(int socketID)
{

    if (!QSslSocket::supportsSsl()) {
        qDebug()<<"QSslSocket dont support ssl";
        return false;
    }
    qDebug()<<"create new ssl socket";
    this->sslSocket = new QSslSocket(this);

    qDebug()<<"set socket Descriptor";

    /* set handle for the socket */
    if (this->sslSocket->setSocketDescriptor(socketID)) {

	qDebug()<<"set connect signal";
        /* installation of signaling for the socket */
        connect(
                    this->sslSocket,
                    SIGNAL(encrypted()),
                    this,
                    SLOT(encrypted())
                );
        connect(
                    this->sslSocket,
                    SIGNAL(readyRead()),
                    this,
                    SLOT(readyRead())
               );
        connect(
                    this->sslSocket,
                    SIGNAL (disconnected()),
                    this,
                    SLOT(disconnected())
                );
        connect(
                    this->sslSocket,
                    SIGNAL(connected()),
                    this,
                    SLOT(connected())
                );
        connect(
                    this->sslSocket,
                    SIGNAL(sslErrors(const QList<QSslError> &)),
                    this,
                    SLOT(socketSslErrors(const QList<QSslError> &))
                );
        connect(
                    this->sslSocket,
                    SIGNAL(error(QAbstractSocket::SocketError)),
                    this,
                    SLOT(socketError(QAbstractSocket::SocketError))
                );



	qDebug()<<"set protocol key";
        /* setup key */
        this->sslSocket->setLocalCertificate("key072.pem");
        this->sslSocket->setPrivateKey("key072.key");
        /* srup ssl version*/
        this->sslSocket->setProtocol(QSsl::TlsV1_2);

        qDebug()<<"start server encryptions";

        this->sslSocket->startServerEncryption();

        this->socketID = socketID;

        qDebug() << "socket setup";

        return true;
    } else {

        qDebug()<< "error setup socket "<<this->sslSocket->errorString() ;
        delete this->sslSocket;
        return false;
    }
}

void ClientConnection::readyRead()
{
    qDebug()<<this->socketID <<"ready read: "<<this->sslSocket->readAll();
}



void ClientConnection::connected()
{
    qDebug()<<"clent connected";
}

void ClientConnection::disconnected()
{
    qDebug() <<"client disconected: "<<this->socketID;

    emit disconnectedSocket(this->socketID);
}

void ClientConnection::encrypted()
{
    qDebug() <<"encrypted";
}

void ClientConnection::socketSslErrors(const QList<QSslError> & list)
{
    qDebug()<<"soket ssl error";
    /*  display all error */
    foreach (QSslError item, list) {
        qDebug()<<item.errorString();
    }

}

void ClientConnection::sendData(int command, const QByteArray &data)
{
    qDebug()<<"send data to socket:"<<this->socketID<<" "<<data.size();

    /* generation of data to send */
    QByteArray dataToSend;
    QDataStream sendStream(&dataToSend,QIODevice::WriteOnly);
    /*  reserve a place in the size of the package and rewrite data */
    sendStream << quint64(0) <<quint32(command)<<data;
    /* Go to the beginning of the packet data */
    sendStream.device()->seek(0);
    /* rewrite reserved a place the size of the package */
    sendStream << quint64(dataToSend.size() - sizeof(quint64));
    /* send data over the network */
    int count = this->sslSocket->write(dataToSend);
    // qDebug()<<"send:"<<count;
}

void ClientConnection::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "socketError: "<<error;
    //emit disconnectedSocket(this->socketID);
}
