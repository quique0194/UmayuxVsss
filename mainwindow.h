#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QString>
#include "readcamerathread.h"
#include "calibration.h"

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

    void change_current_calib(QString calib);

    void new_calib(Calibration* calib);

    void on_slider0_sliderMoved(int position);
    void on_slider1_sliderMoved(int position);
    void on_slider2_sliderMoved(int position);
    void on_slider3_sliderMoved(int position);
    void on_slider4_sliderMoved(int position);
    void on_slider5_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    Calibration blueCalib;
    Calibration yellowCalib;
    Calibration orangeCalib;
    Calibration redCalib;
    Calibration greenCalib;
    Calibration skyblueCalib;
    Calibration* currentCalib;
};

#endif // MAINWINDOW_H
