#include "calibration.h"

Calibration::Calibration(int x0, int x1, int y0, int y1, int z0, int z1)
{
    data.assign(6, 0);
    setData(x0, x1, y0, y1, z0, z1);
}

Calibration::~Calibration()
{

}

void Calibration::setData(int x0, int x1, int y0, int y1, int z0, int z1)
{
    data[0] = x0;
    data[1] = y0;
    data[2] = z0;
    data[3] = x1;
    data[4] = y1;
    data[5] = z1;
    saveState();
}

string Calibration::toString()
{
    stringstream ss;
    for (int i = 0; i < 6; ++i) {
        ss << (int)data[i] << " ";
    }
    ss << endl;
    return ss.str();
}

void Calibration::saveState()
{
    vector<uchar> state = data;
    previous.push_back(state);
    if (previous.size() > 10) {
        previous.pop_front();
    }
}

void Calibration::newRegion(QImage img)
{
    Mat mat(img.height(), img.width(), CV_8UC3, (uchar*)img.bits(),img.bytesPerLine());
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
    saveState();
    emit newCalibration(this);
}

void Calibration::setX0(int v)
{
    data[0] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::setX1(int v)
{
    data[3] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::setY0(int v)
{
    data[1] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::setY1(int v)
{
    data[4] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::setZ0(int v)
{
    data[2] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::setZ1(int v)
{
    data[5] = v;
    saveState();
    emit newCalibration(this);
}

void Calibration::undo()
{
    if (previous.size() >= 2) {
        previous.pop_back();
        data = previous.back();
        emit newCalibration(this);
    }
}

