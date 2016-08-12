#include "readcamerathread.h"

ReadCameraThread::ReadCameraThread(): keep_reading(true)
{

}

ReadCameraThread::~ReadCameraThread()
{

}

void ReadCameraThread::run() {
    VideoCapture cap(1);
    while (keep_reading) {
        cap >> frame;
        cvtColor(frame, frame, CV_BGR2RGB);
        emit newFrame(&frame);
    }
}

void ReadCameraThread::customStop() {
    keep_reading = false;
}
