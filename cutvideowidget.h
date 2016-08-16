#ifndef CUTVIDEOWIDGET_H
#define CUTVIDEOWIDGET_H

#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class CutVideoWidget : public QLabel
{
    Q_OBJECT
public:
    CutVideoWidget(QFrame* frame=0);
    ~CutVideoWidget();

    QRect selection;
    QRect roi; // Region of interest
    float factor;
protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    bool selecting;
    QImage* img;
signals:
    void newSelection();
    void newRoi(QRect*);
    void resetRoi();
public slots:
    void setFrame(Mat* frame);
    void cut();
    void reset();
};

#endif // CUTVIDEOWIDGET_H
