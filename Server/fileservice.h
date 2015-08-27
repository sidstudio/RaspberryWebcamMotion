#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>
#include <type.h>
#include <QString>

/**
 * @brief The FileService class
 * @author L.Skrypka sidstudio.com.ua
 */
class FileService : public QObject
{
    Q_OBJECT
public:
    explicit FileService(QObject *parent = 0);

private:

    QString path;

signals:

public slots:
    void getNewFrame(server::ImageData data);
};

#endif // FILESERVICE_H
