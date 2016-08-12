#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rct.start();
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->cutVideo, SLOT(setFrame(Mat*)));
}

MainWindow::~MainWindow()
{
    rct.customStop();
    rct.wait();
    delete ui;
}
