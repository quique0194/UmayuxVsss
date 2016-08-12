#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include "readcamerathread.h"

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
