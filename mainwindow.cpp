#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);

    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->showVision, SLOT(proc(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->showVision, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->cutVideo, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->selectArea, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->showBinarized, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFPS(QString)), ui->statusBar, SLOT(showMessage(QString)));

    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), &rct, SLOT(setRoi(QRect*)));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), &rct, SLOT(resetRoi()));
    QObject::connect(ui->cutVideo, SIGNAL(newSelection()), ui->cutBtn, SLOT(enable()));
    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), ui->cutBtn, SLOT(disable()));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), ui->cutBtn, SLOT(disable()));

    QObject::connect(ui->selectArea, SIGNAL(newArea(QImage)), ui->showFiltered, SLOT(setImage(QImage)));
    QObject::connect(ui->selectArea, SIGNAL(newArea(QImage)), &calibHandler, SLOT(newRegion(QImage)));

    QObject::connect(ui->currentCalibColor, SIGNAL(currentIndexChanged(QString)), &calibHandler, SLOT(changeCurrentCalib(QString)));

    QObject::connect(&calibHandler, SIGNAL(newCalibration(Calibration*)), this, SLOT(setSliders(Calibration*)));
    QObject::connect(&calibHandler, SIGNAL(newCalibration(Calibration*)), ui->showBinarized, SLOT(newCalibration(Calibration*)));

    QObject::connect(ui->slider0, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setX0(int)));
    QObject::connect(ui->slider1, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setX1(int)));
    QObject::connect(ui->slider2, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setY0(int)));
    QObject::connect(ui->slider3, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setY1(int)));
    QObject::connect(ui->slider4, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setZ0(int)));
    QObject::connect(ui->slider5, SIGNAL(sliderMoved(int)), &calibHandler, SLOT(setZ1(int)));

    QObject::connect(ui->exposureSlider, SIGNAL(sliderMoved(int)), this, SLOT(exposureSliderChange(int)));

    QObject::connect(ui->resetSliders01, SIGNAL(clicked()), &calibHandler, SLOT(resetX()));
    QObject::connect(ui->resetSliders23, SIGNAL(clicked()), &calibHandler, SLOT(resetY()));
    QObject::connect(ui->resetSliders45, SIGNAL(clicked()), &calibHandler, SLOT(resetZ()));

    QObject::connect(ui->saveCalibBtn, SIGNAL(clicked()), &calibHandler, SLOT(save()));
    QObject::connect(ui->resetAreaBtn, SIGNAL(clicked()), ui->selectArea, SLOT(reset()));
    QObject::connect(ui->undoBtn, SIGNAL(clicked()), &calibHandler, SLOT(undo()));

    ui->showVision->setCalibrationHandler(&calibHandler);
    emit calibHandler.newCalibration(calibHandler.currentCalib);
    rct.start();

    //// EXTERNAL COMMANDS
    QStringList params;
    params << "-u";     // -u makes writes to stdout unbuffered
    params << "E:\\codigos\\borrame\\strategy.py";
    strategyCmd = new AsyncCmd(QString("python"), params);
    strategyCmd->start();

    QStringList paramsControl;
    paramsControl << "-u";     // -u makes writes to stdout unbuffered
    paramsControl << "C:\\Users\\victo\\Documents\\ConexionBluetooth\\remote_control.py";
    controlCmd = new AsyncCmd(QString("python"), paramsControl);
    controlCmd->start();

    connect(strategyCmd, SIGNAL(newText(QString)), ui->strategyOutput, SLOT(appendText(QString)));
    connect(strategyCmd, SIGNAL(cmd_started()), ui->strategyOutput, SLOT(clear()));
    connect(strategyCmd, SIGNAL(cmd_finished()), ui->strategyOutput, SLOT(printFinished()));
    connect(strategyCmd, SIGNAL(cmd_started()), ui->startStrategy, SLOT(disable()));
    connect(strategyCmd, SIGNAL(cmd_finished()), ui->startStrategy, SLOT(enable()));

    connect(controlCmd, SIGNAL(newText(QString)), ui->controlOutput, SLOT(appendText(QString)));
    connect(controlCmd, SIGNAL(cmd_started()), ui->controlOutput, SLOT(clear()));
    connect(controlCmd, SIGNAL(cmd_finished()), ui->controlOutput, SLOT(printFinished()));
    connect(controlCmd, SIGNAL(cmd_started()), ui->startControl, SLOT(disable()));
    connect(controlCmd, SIGNAL(cmd_finished()), ui->startControl, SLOT(enable()));
}

MainWindow::~MainWindow()
{
    rct.customStop();
    strategyCmd->terminate_thread();
    controlCmd->terminate_thread();
    rct.wait();
    strategyCmd->wait();
    controlCmd->wait();
    delete strategyCmd;
    delete controlCmd;
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


void MainWindow::setSliders(Calibration *calib)
{
    ui->slider0->setSliderPosition(calib->data[0]);
    ui->slider1->setSliderPosition(calib->data[3]);
    ui->slider2->setSliderPosition(calib->data[1]);
    ui->slider3->setSliderPosition(calib->data[4]);
    ui->slider4->setSliderPosition(calib->data[2]);
    ui->slider5->setSliderPosition(calib->data[5]);
}

void MainWindow::exposureSliderChange(int val)
{
    rct.cap->set(CV_CAP_PROP_EXPOSURE, val);
}

/*
    ===========================================================
    RUN COMMANDS
    ===========================================================
*/

void MainWindow::on_startStrategy_clicked()
{
    strategyCmd->run_cmd();
}


void MainWindow::on_startControl_clicked()
{
    controlCmd->run_cmd();
}

/*
    ===========================================================
    END RUN COMMANDS
    ===========================================================
*/
