#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->cutVideo, SLOT(setFrame(Mat*)));
    QObject::connect(&rct, SIGNAL(newFrame(Mat*)), ui->selectArea, SLOT(setFrame(Mat*)));

    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), &rct, SLOT(setRoi(QRect*)));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), &rct, SLOT(resetRoi()));
    QObject::connect(ui->cutVideo, SIGNAL(newSelection()), ui->cutBtn, SLOT(enable()));
    QObject::connect(ui->cutVideo, SIGNAL(newRoi(QRect*)), ui->cutBtn, SLOT(disable()));
    QObject::connect(ui->cutVideo, SIGNAL(resetRoi()), ui->cutBtn, SLOT(disable()));

    QObject::connect(ui->selectArea, SIGNAL(newArea(QPixmap)), ui->showFiltered, SLOT(setPixmap(QPixmap)));
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
