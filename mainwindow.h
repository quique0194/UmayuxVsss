#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QtConcurrent/QtConcurrent>
#include "readcamerathread.h"
#include "asynccmd.h"
#include "calibrationhandler.h"

using namespace std;
using namespace QtConcurrent;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ReadCameraThread rct;
    AsyncCmd* strategyCmd;
    AsyncCmd* controlCmd;

private slots:
    void on_cutBtn_clicked();
    void on_resetBtn_clicked();
    void on_selectAreaBtn_clicked();
    void setSliders(Calibration* calib);
    void exposureSliderChange(int val);

    void on_startStrategy_clicked();

    void on_startControl_clicked();

private:
    Ui::MainWindow *ui;
    CalibrationHandler calibHandler;
};

#endif // MAINWINDOW_H
