#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <algorithm>
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Calibration : public QObject
{
    Q_OBJECT
public:
    Calibration();
    ~Calibration();
    uchar data[6];
signals:
    void newCalibration(Calibration* cal);
public slots:
    void newRegion(QImage img);
};

#endif // CALIBRATION_H
