#include "readcamerathread.h"

ReadCameraThread::ReadCameraThread(): keep_reading(true), roi_exists(false)
{

}

ReadCameraThread::~ReadCameraThread()
{
    if (frame) {
        delete frame;
    }
}

void ReadCameraThread::run() {
    VideoCapture cap(0);
    while (keep_reading) {
        Mat img;
        cap >> img;
        if (img.data == NULL || img.cols == 0 || img.rows == 0) {
            cout << "Camera returned null data" << endl;
            continue;
        }
        cvtColor(img, img, CV_BGR2RGB);
        if (roi_exists) {
            int x1 = roi.topLeft().x();
            int y1 = roi.topLeft().y();
            int x2 = roi.bottomRight().x();
            int y2 = roi.bottomRight().y();
            Rect cv_roi(x1, y1, x2-x1, y2-y1);
            Mat cropped = img(cv_roi);
            cropped.copyTo(img);
        }
        if (frame) {
            delete frame;
        }
        frame = new Mat(img);
        emit newFrame(frame);
    }
    cout << "End of read camera thread" << endl;
}

void ReadCameraThread::setRoi(QRect *new_roi)
{
    roi_exists = true;
    roi = *new_roi;
}

void ReadCameraThread::resetRoi()
{
    roi_exists = false;
}

void ReadCameraThread::customStop() {
    keep_reading = false;
}
