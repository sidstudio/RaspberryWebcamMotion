#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H


#include <QSslSocket>
#include <type.h>

/**
 * @brief The ClientConnection class
 * @author L.Skrypka sidstudio.com.ua
 *
 * Class to handle client connections.
 */
class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QObject *parent = 0);

    bool setSocket(int socketID);

private:
    QSslSocket *sslSocket;
    qintptr socketID;

signals:
    /* sent signal when disconnecting the socket */
    void disconnectedSocket(qintptr socketID);

public slots:
    void encrypted();
    void connected();
    void disconnected();
    void socketError(QAbstractSocket::SocketError error);
    void socketSslErrors(const QList<QSslError> &list);
    void readyRead();

    void sendData(int comand,const QByteArray &data);

};

#endif // CLIENTCONNECTION_H
