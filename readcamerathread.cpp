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
//    cap->set(CV_CAP_PROP_FRAME_WIDTH,352);
//    cap->set(CV_CAP_PROP_FRAME_HEIGHT,288);

    cap->set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT,480);

//    cap->set(CV_CAP_PROP_FRAME_WIDTH,640);
//    cap->set(CV_CAP_PROP_FRAME_HEIGHT,360);

    cout << "WIDTH " << cap->get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    cout << "HEIGHT " << cap->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
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
        else {
            resize(img, img, Size(480, 360));
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
        if (frames.size() > 300) {
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
