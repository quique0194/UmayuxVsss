#ifndef SHOWVISIONWIDGET_H
#define SHOWVISIONWIDGET_H

#include <iostream>
#include <string>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QPoint>
#include <QTime>
#include <opencv2/opencv.hpp>
#include "calibrationhandler.h"

using namespace std;
using namespace cv;


class ShowVisionWidget : public QLabel
{
    Q_OBJECT
public:
    ShowVisionWidget(QWidget* parent=0);
    ~ShowVisionWidget();
    QSize sizeHint() const;
protected:
    void paintEvent(QPaintEvent*);
private:
    bool validPoint(const Point& p);
    Point find_ball(Mat);
    QImage* img;
    CalibrationHandler* ch;
    vector<Point> my_team;
    vector<Point> my_team_ori;
    vector<Point> op_team;
    Point ball;
public slots:
    void reset();
    void proc(Mat* frame);
    void setFrame(Mat* frame);
    void setCalibrationHandler(CalibrationHandler* ch);
};

#endif // SHOWVISIONWIDGET_H
