#ifndef SHOWINRANGEWIDGET_H
#define SHOWINRANGEWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include "calibration.h"

class ShowInRangeWidget : public QLabel
{
    Q_OBJECT
public:
    ShowInRangeWidget(QWidget* parent);
    ~ShowInRangeWidget();
    void paintEvent(QPaintEvent*);
    Calibration* calib;
    QSize sizeHint() const;
private:
    QImage* img;
public slots:
    void newCalibration(Calibration* calib);
    void setFrame(Mat *frame);
};

#endif // SHOWINRANGEWIDGET_H
