#ifndef PI2SSLSERVER_H
#define PI2SSLSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <iostream>
#include "clientconnection.h"
#include <vector>
#include "type.h"
#include "videocapture.h"
#include <QMap>
#include <pooldetections.h>
#include <fileservice.h>

/**
 * @brief The PI2SslServer class
 * @author L.Skrypka sidstudio.com.ua
 *
 * Server for RaspberryPi working through Ssl sockets.
 * the server accepts connections on port number
 * and creates a thread to handle the connection.
 */
class PI2SslServer : public QTcpServer
{
    Q_OBJECT

public:

    /**
     * @brief PI2SslServer
     * @param port
     * @param parent
     */
    explicit PI2SslServer(int port,QObject *parent = 0);

    ~PI2SslServer();

    /**
     * @brief startServer
     * @return bool
     *
     * The function starts the server on the specified port.
     * Returns false if the server fails to start
     */
    bool startServer();

private:
    /**
     * @brief incomingConnection
     * @param handle
     *
     * The function accepts a new connection to the server
     */
    void incomingConnection(qintptr handle);

    /* TODO: the maximum number of connections connected */
    static const int MAX_CONNECTION_COUNT = 50;

    /* port number on which the server is running  */
    int connectingPort;

    QThread* signalThread;

    /* capture video from your camera */
    camera::VideoCapture *videoCapture;

    QThread *threadCapture;

    /*
     * Collection that stores the netlist
     * qintptr is a connection socket descriptor
     * ClientConnection pointer to the client connection
     */
    QMap<qintptr, ClientConnection*> clientListConnextions;
    /* thread list  */
    QMap<qintptr, QThread*> clientThreadConnections;

    PoolDetections* poolDetections;
    QThread* threadPoolDetections;

    FileService fileService;

signals:

    void serverError(server::ServerSignal error);

    /* start capture from the camera */
    void startCapture();
    /* stop capture from the camera */
    void stopCapture();



public slots:

    void clientDisconnected(qintptr socketID);

};

#endif // PI2SSLSERVER_H
