#ifndef CALIBRATIONHANDLER_H
#define CALIBRATIONHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <QObject>
#include <QString>
#include "calibration.h"

using namespace std;


class CalibrationHandler : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationHandler();
    ~CalibrationHandler();
    string toString();
    Calibration* currentCalib;
private:
    Calibration blueCalib;
    Calibration yellowCalib;
    Calibration orangeCalib;
    Calibration redCalib;
    Calibration greenCalib;
    Calibration skyblueCalib;
signals:
    void newCalibration(Calibration* calib);
public slots:
    void save();
    void changeCurrentCalib(QString calib);
    void emitNewCalibration(Calibration* calib);
    void newRegion(QImage img);
    void setX0(int);
    void setX1(int);
    void setY0(int);
    void setY1(int);
    void setZ0(int);
    void setZ1(int);

    void resetX();
    void resetY();
    void resetZ();
public slots:
};

#endif // CALIBRATIONHANDLER_H
