#include <QCoreApplication>
#include "pi2sslserver.h"

#include <iostream>
#include <QThread>
#include <QObject>
#include "type.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PI2SslServer *server= new PI2SslServer(1258);
    if (!server->startServer()) {
        return -1;
    }

    return a.exec();
}
