#ifndef READCAMERATHREAD_H
#define READCAMERATHREAD_H

#include <iostream>
#include <QtCore>
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

    Mat frame;
signals:
    void newFrame(Mat* frame);
private:
    void run();

    bool keep_reading;
};

#endif // READCAMERATHREAD_H
