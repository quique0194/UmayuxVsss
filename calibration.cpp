#include "calibration.h"

Calibration::Calibration()
{
    data[0] = 255;
    data[1] = 255;
    data[2] = 255;
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
}

Calibration::~Calibration()
{

}

void Calibration::newRegion(QImage img)
{
    Mat mat(img.size().height(), img.size().width(), CV_8UC3, img.scanLine(0));
    for(int i = 0; i < mat.rows; ++i) {
        uchar* p = mat.ptr<uchar>(i);
        for (int j = 0; j < mat.cols; ++j) {
            int base = 3 * j;
            uchar& x = p[base];
            uchar& y = p[base + 1];
            uchar& z = p[base + 2];
            data[0] = min(x, data[0]);
            data[1] = min(y, data[1]);
            data[2] = min(z, data[2]);
            data[3] = max(x, data[3]);
            data[4] = max(y, data[4]);
            data[5] = max(z, data[5]);
        }
    }
    emit newCalibration(this);
}

