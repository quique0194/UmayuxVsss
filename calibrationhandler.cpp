#include "calibrationhandler.h"

CalibrationHandler::CalibrationHandler()
{
    ifstream file("calibration.txt");
    if (file.good()) {
        int x0, y0, z0, x1, y1, z1;
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        blueCalib.setData(x0, x1, y0, y1, z0, z1);
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        yellowCalib.setData(x0, x1, y0, y1, z0, z1);
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        orangeCalib.setData(x0, x1, y0, y1, z0, z1);
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        redCalib.setData(x0, x1, y0, y1, z0, z1);
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        greenCalib.setData(x0, x1, y0, y1, z0, z1);
        file >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
        skyblueCalib.setData(x0, x1, y0, y1, z0, z1);
    }
    file.close();

    QObject::connect(&blueCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));
    QObject::connect(&yellowCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));
    QObject::connect(&orangeCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));
    QObject::connect(&redCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));
    QObject::connect(&greenCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));
    QObject::connect(&skyblueCalib, SIGNAL(newCalibration(Calibration*)), this, SLOT(emitNewCalibration(Calibration*)));

    currentCalib = &blueCalib;
}

CalibrationHandler::~CalibrationHandler()
{

}

string CalibrationHandler::toString()
{

    stringstream ss;
    ss << blueCalib.toString();
    ss << yellowCalib.toString();
    ss << orangeCalib.toString();
    ss << redCalib.toString();
    ss << greenCalib.toString();
    ss << skyblueCalib.toString();
    return ss.str();
}

void CalibrationHandler::save()
{
    ofstream f;
    f.open("calibration.txt");
    f << toString();
    f.close();
}

void CalibrationHandler::changeCurrentCalib(QString calib)
{
    if (calib.toStdString() == "Blue") {
        currentCalib = &blueCalib;
    } else if (calib.toStdString() == "Yellow") {
        currentCalib = &yellowCalib;
    } else if (calib.toStdString() == "Orange") {
        currentCalib = &orangeCalib;
    } else if (calib.toStdString() == "Red") {
        currentCalib = &redCalib;
    } else if (calib.toStdString() == "Green") {
        currentCalib = &greenCalib;
    } else if (calib.toStdString() == "Skyblue") {
        currentCalib = &skyblueCalib;
    } else {
        cout << "No se encuentra calibracion para este color" << endl;
    }
    emit newCalibration(currentCalib);
}

void CalibrationHandler::emitNewCalibration(Calibration *calib)
{
    emit newCalibration(calib);
}

void CalibrationHandler::newRegion(QImage img)
{
    currentCalib->newRegion(img);
}

void CalibrationHandler::setX0(int v)
{
    currentCalib->setX0(v);
}

void CalibrationHandler::setX1(int v)
{
    currentCalib->setX1(v);
}

void CalibrationHandler::setY0(int v)
{
    currentCalib->setY0(v);
}

void CalibrationHandler::setY1(int v)
{
    currentCalib->setY1(v);
}

void CalibrationHandler::setZ0(int v)
{
    currentCalib->setZ0(v);
}

void CalibrationHandler::setZ1(int v)
{
    currentCalib->setZ1(v);
}

