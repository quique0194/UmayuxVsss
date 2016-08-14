#ifndef READCAMERATHREAD_H
#define READCAMERATHREAD_H

#include <iostream>
#include <queue>
#include <QtCore>
#include <QRect>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class ReadCameraThread : public QThread
{
    Q_OBJECT
public:
    ReadCameraThread();
    ~ReadCameraThread();
    void customStop();

    queue<Mat*> frames;
private:
    void run();

    bool keep_reading;
    bool roi_exists;
    QRect roi;
signals:
    void newFrame(Mat* frame);
public slots:
    void setRoi(QRect* new_roi);
    void resetRoi();
};

#endif // READCAMERATHREAD_H
