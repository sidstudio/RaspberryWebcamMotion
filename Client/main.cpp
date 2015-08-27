#include "dialog.h"
#include <QApplication>
#include <QWindow>
#include <QScreen>
#include <QDesktopWidget>
#include <QDebug>
#include "mainwindow.h"
#include <QMenuBar>
#include "connection.h"
#include <QThread>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <pi2client.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

    PI2Client* client = new PI2Client();

    QRect rect = QApplication::desktop()->screenGeometry();

    client->setWindowSize(rect);

    QObject::connect(
                    client,
                    SIGNAL(closeClient()),
                    QApplication::instance(),
                    SLOT(quit())
                );

    client->startClient();



    return a.exec();
}
