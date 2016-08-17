#ifndef SHOWVISIONWIDGET_H
#define SHOWVISIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <opencv2/opencv.hpp>
#include "calibrationhandler.h"

using namespace cv;


class ShowVisionWidget : public QLabel
{
    Q_OBJECT
public:
    ShowVisionWidget(QWidget* parent=0);
    ~ShowVisionWidget();
    QSize sizeHint() const;
    void setCalibrationHandler(CalibrationHandler* ch);
protected:
    void paintEvent(QPaintEvent*);
private:
    vector<Point> proc(Mat* frame);
    QImage* img;
    CalibrationHandler* ch;
public slots:
    void setFrame(Mat* frame);
};

#endif // SHOWVISIONWIDGET_H
