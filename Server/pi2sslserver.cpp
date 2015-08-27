#include "pi2sslserver.h"

PI2SslServer::PI2SslServer(int port, QObject *parent) :
    QTcpServer(parent)
{

    this->connectingPort = port;
    this->signal = new AppSignal();
    this->signalThread = new QThread();

    signal->moveToThread(signalThread);

    signalThread->start();
    /*
    QObject::connect(
                    signalThread,
                    SIGNAL(started()),
                    signal,
                    SLOT(input())
                );
    */


}

PI2SslServer::~PI2SslServer()
{


    delete this->signal;

    delete poolDetections;

    close();
}

/**
 * @brief PI2SslServer::startServer
 * @return bool
 */
bool PI2SslServer::startServer()
{
    /*
     * Start the server on the specified port.
     * If the server can not run it sends a signal SERVER_START_ERROR
     *
     */
    if (listen(QHostAddress::Any,this->connectingPort)) {
        std::cout<<"Server start on port: "<<this->connectingPort<<std::endl;
    } else {
        /* diplay error */
        std::cout<<"Can`t start server on port: "<<this->connectingPort<<" error:"<<errorString().toStdString().c_str()<<std::endl;
        /* sent signal */
        emit serverError(server::ServerSignal::SERVER_START_ERROR);
        return false;
    }

    this->clientListConnextions.clear();
    this->clientThreadConnections.clear();

    this->poolDetections = new PoolDetections();
    this->threadPoolDetections = new QThread();
    this->poolDetections->moveToThread(this->threadPoolDetections);

    qDebug()<<"test";

    QObject::connect(
                    this->threadPoolDetections,
                    SIGNAL(started()),
                    this->poolDetections,
                    SLOT(startRecognitions()),
                    Qt::DirectConnection
                );

    QObject::connect(
                    this->poolDetections,
                    SIGNAL(sendDetectionFrame(server::ImageData)),
                    &fileService,
                    SLOT(getNewFrame(server::ImageData)),
                    Qt::DirectConnection
                );
    this->threadPoolDetections->start();

    this->videoCapture = new camera::VideoCapture();

    QObject::connect(
                    this->videoCapture,
                    SIGNAL(frameToDetections(server::ImageData)),
                    this->poolDetections,
                    SLOT(imageToDetections(server::ImageData)),
                    Qt::DirectConnection
                );

    this->threadCapture = new QThread();
    this->videoCapture->moveToThread(this->threadCapture);


    QObject::connect(
                    this->threadCapture,
                    SIGNAL(started()),
                    this->videoCapture,
                    SLOT(startCapture())
                );

    QObject::connect(
                    videoCapture,
                    SIGNAL(finish()),
                    threadCapture,
                    SLOT(quit())
                );

    QObject::connect(
                    videoCapture,
                    SIGNAL(finish()),
                    videoCapture,
                    SLOT(deleteLater())
                );

    QObject::connect(
                    threadCapture,
                    SIGNAL(finished()),
                    threadCapture,
                    SLOT(deleteLater())
                );



    QObject::connect(
                    this,
                    SIGNAL(stopCapture()),
                    this->videoCapture,
                    SLOT(stopCapture()),
                    Qt::DirectConnection
                );

    QObject::connect(
                    this,
                    SIGNAL(startCapture()),
                    this->videoCapture,
                    SLOT(startCapture()),
                    Qt::DirectConnection
                );


    /* starting a thread for camera capture  */
    threadCapture->start();

    //emit startCapture();

    return true;
}

/**
 * @brief PI2SslServer::incomingConnection
 * @param handle
 */
void PI2SslServer::incomingConnection(qintptr handle)
{

    qDebug()<<"incomingConnection:"<<handle;

    /* It accepts incoming connections on the socket descriptor */
    ClientConnection *newConnection = new ClientConnection();
    qDebug()<<"setSoket";
    if (newConnection->setSocket(handle)) {
        /*
         * Associate getting a new frame for the sending socket
         * Each time the camera has a frame to send a signal is sent newFrame.
         * Compound current handler receives this signal and sends it over the network
         */
        /*
        QObject::connect(
                        this->videoCapture,
                        SIGNAL(newFrame(QByteArray)),
                        newConnection,
                        SLOT(sendData(QByteArray))
                    );
        */
        QObject::connect(
                        this->videoCapture,
                        SIGNAL(newRawFrame(int,QByteArray)),
                        newConnection,
                        SLOT(sendData(int,QByteArray))
                    );
        QObject::connect(
                    this->videoCapture,
                    SIGNAL(newDetectionFrame(int,QByteArray)),
                    newConnection,
                    SLOT(sendData(int,QByteArray))
                    );
        QObject::connect(
                        this->poolDetections,
                        SIGNAL(newFrame(int,QByteArray)),
                        newConnection,
                        SLOT(sendData(int,QByteArray))
                    );


        QObject::connect(
                    newConnection,
                    SIGNAL(disconnectedSocket(qintptr)),
                    this,
                    SLOT(clientDisconnected(qintptr)),
                     Qt::DirectConnection
                    );


        this->clientListConnextions.insert(handle,newConnection);

        QThread *t = new QThread();
        newConnection->moveToThread(t);


        /*
        QObject::connect(
                        t,
                        SIGNAL(finished()),
                        t,
                        SLOT(deleteLater())
                    );


        QObject::connect(
                    newConnection,
                    SIGNAL(destroyed(QObject*)),
                    t,
                    SLOT(deleteLater())
                    );

        QObject::connect(
                    newConnection,
                    SIGNAL(destroyed(QObject*)),
                    t,
                    SLOT(quit())
                    );
        */

        t->start();

        clientThreadConnections.insert(handle,t);

    } else {
        qDebug()<<"socket setup error";
    }

}


void PI2SslServer::clientDisconnected(qintptr socketID)
{

    qDebug()<<"client disconected from server:"<<socketID;
    ClientConnection* clientConnection = this->clientListConnextions.take(socketID);

    /*
    QObject::disconnect(
                this->videoCapture,
                SIGNAL(newFrame(QByteArray)),
                clientConnection,
                SLOT(sendData(QByteArray))
                );
    */
    QObject::disconnect(
                this->videoCapture,
                SIGNAL(newDetectionFrame(int,QByteArray)),
                clientConnection,
                SLOT(sendData(int,QByteArray))
                );
    QObject::disconnect(
                    this->poolDetections,
                    SIGNAL(newFrame(int,QByteArray)),
                    clientConnection,
                    SLOT(sendData(int,QByteArray))
                );
    QObject::disconnect(
                    this->videoCapture,
                    SIGNAL(newRawFrame(int,QByteArray)),
                    clientConnection,
                    SLOT(sendData(int,QByteArray))
                );



    this->clientListConnextions.remove(socketID);

    QThread *t = this->clientThreadConnections.take(socketID);

    if (t->isRunning()) {
        t->requestInterruption();
        // t->wait();
        t->quit();
        t->deleteLater();
    }

    //delete t;

    //delete clientConnection;
}

