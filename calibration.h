#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <algorithm>
#include <string>
#include <sstream>
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Calibration : public QObject
{
    Q_OBJECT
public:
    Calibration(int x0=255, int x1=0, int y0=255, int y1=0, int z0=255, int z1=0);
    ~Calibration();
    void setData(int x0=255, int x1=0, int y0=255, int y1=0, int z0=255, int z1=0);
    string toString();
    uchar data[6];
signals:
    void newCalibration(Calibration* cal);
public slots:
    void newRegion(QImage img);
    void setX0(int);
    void setX1(int);
    void setY0(int);
    void setY1(int);
    void setZ0(int);
    void setZ1(int);
};

#endif // CALIBRATION_H
