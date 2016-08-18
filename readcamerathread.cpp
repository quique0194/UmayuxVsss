#include "readcamerathread.h"

ReadCameraThread::ReadCameraThread(): keep_reading(true), roi_exists(false)
{
    fps = 0;
    cap = 0;
}

ReadCameraThread::~ReadCameraThread()
{
    delete cap;
    while (!frames.empty()) {
        delete frames.front();
        frames.pop();
    }
}

void ReadCameraThread::run() {
    cap = new VideoCapture(0);
    QTime time;
    time.start();
    while (keep_reading) {
        Mat img;
        img.data = NULL;
        *cap >> img;
        if (img.empty()) {
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
        if (frames.size() > 1000) {
            delete frames.front();
            frames.pop();
        }
        Mat* ptr = new Mat(img);
        frames.push(ptr);
        emit newFrame(ptr);
        // FPS
        if (time.elapsed() >= 1000) {
            emit newFPS(fps);
            emit newFPS("FPS: " + QString::number(fps));
            fps = 0;
            time.restart();
        } else {
            fps++;
        }
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
