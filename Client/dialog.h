#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QSystemTrayIcon>
#include <QList>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:

    void getNewFrameFromServer(int type,cv::Mat& image);

private slots:
    void on_pushButton_clicked();

    void on_pushButtonPrev_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;
    QList<cv::Mat> listImage;
    int curentImagePos;

};

#endif // DIALOG_H
