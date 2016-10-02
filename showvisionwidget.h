#ifndef SHOWVISIONWIDGET_H
#define SHOWVISIONWIDGET_H

#include <math.h>
#include <iostream>
#include <string>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QPoint>
#include <QTime>
#include <QtNetwork/QtNetwork>
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
    bool validPoint(const Point2f& p);
    Point2f point2Strategy(const Point2f& p);
    float getOrientation(const Point2f& a, const Point2f& b);
    Point2f find_ball(Mat);
    QImage* img;
    CalibrationHandler* ch;
    vector<Point2f> my_team;
    vector<Point2f> my_team_ori;
    vector<Point2f> op_team;
    Point2f ball;
    QUdpSocket socket;
public slots:
    void reset();
    void proc(Mat* frame);
    void setFrame(Mat* frame);
    void setCalibrationHandler(CalibrationHandler* ch);
};

#endif // SHOWVISIONWIDGET_H
