#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QString>
#include "readcamerathread.h"
#include "calibrationhandler.h"

using namespace std;


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

private slots:
    void on_cutBtn_clicked();

    void on_resetBtn_clicked();

    void on_selectAreaBtn_clicked();

    void setSliders(Calibration* calib);

private:
    Ui::MainWindow *ui;
    CalibrationHandler calibHandler;
};

#endif // MAINWINDOW_H
