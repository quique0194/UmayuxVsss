#ifndef CUTVIDEOWIDGET_H
#define CUTVIDEOWIDGET_H

#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

using namespace cv;


class CutVideoWidget : public QLabel
{
    Q_OBJECT
public:
    CutVideoWidget();
    CutVideoWidget(QFrame* frame);
    ~CutVideoWidget();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
private:
    QRect rect;
public slots:
    void setFrame(Mat* frame);
};

#endif // CUTVIDEOWIDGET_H
