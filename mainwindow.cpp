#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentCalib = &blueCalib;

    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->cutVideo, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->selectArea, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->showBinarized, SLOT(setFrame(Mat*)));

    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), &rct, SLOT(setRoi(QRect*)));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), &rct, SLOT(resetRoi()));
    QObject::connect(ui->cutVideo, SIGNAL(newSelection()), ui->cutBtn, SLOT(enable()));
    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), ui->cutBtn, SLOT(disable()));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), ui->cutBtn, SLOT(disable()));

    QObject::connect(ui->selectArea, SIGNAL(newArea(QImage)), ui->showFiltered, SLOT(setImage(QImage)));
    QObject::connect(ui->selectArea, SIGNAL(newArea(QImage)), currentCalib, SLOT(newRegion(QImage)));

    QObject::connect(ui->currentCalibColor, SIGNAL(currentIndexChanged(QString)), this, SLOT(change_current_calib(QString)));

    QObject::connect(currentCalib, SIGNAL(newCalibration(Calibration*)),
                     this, SLOT(new_calib(Calibration*)));
    QObject::connect(currentCalib, SIGNAL(newCalibration(Calibration*)),
                     ui->showBinarized, SLOT(newCalibration(Calibration*)));
    rct.start();
}

MainWindow::~MainWindow()
{
    rct.customStop();
    rct.wait();
    delete ui;
}

void MainWindow::on_cutBtn_clicked()
{
    ui->cutVideo->cut();
}

void MainWindow::on_resetBtn_clicked()
{
    ui->cutVideo->reset();
}

void MainWindow::on_selectAreaBtn_clicked()
{
    ui->selectArea->selectArea();
}

void MainWindow::change_current_calib(QString calib)
{
    cout << "calib " << calib.toStdString() << endl;

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
    }
    new_calib(currentCalib);
}

void MainWindow::new_calib(Calibration *calib)
{
    ui->slider0->setSliderPosition(calib->data[0]);
    ui->slider1->setSliderPosition(calib->data[3]);
    ui->slider2->setSliderPosition(calib->data[1]);
    ui->slider3->setSliderPosition(calib->data[4]);
    ui->slider4->setSliderPosition(calib->data[2]);
    ui->slider5->setSliderPosition(calib->data[5]);
}

void MainWindow::on_slider0_sliderMoved(int position)
{
    currentCalib->data[0] = position;
    emit currentCalib->newCalibration(currentCalib);
}

void MainWindow::on_slider1_sliderMoved(int position)
{
    currentCalib->data[3] = position;
    emit currentCalib->newCalibration(currentCalib);
}

void MainWindow::on_slider2_sliderMoved(int position)
{
    currentCalib->data[1] = position;
    emit currentCalib->newCalibration(currentCalib);
}

void MainWindow::on_slider3_sliderMoved(int position)
{
    currentCalib->data[4] = position;
    emit currentCalib->newCalibration(currentCalib);
}

void MainWindow::on_slider4_sliderMoved(int position)
{
    currentCalib->data[2] = position;
    emit currentCalib->newCalibration(currentCalib);
}

void MainWindow::on_slider5_sliderMoved(int position)
{
    currentCalib->data[5] = position;
    emit currentCalib->newCalibration(currentCalib);
}
