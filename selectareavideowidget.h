#ifndef SELECTAREAVIDEOWIDGET_H
#define SELECTAREAVIDEOWIDGET_H

#include <cmath>
#include <algorithm>
#include <QLabel>
#include <QFrame>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QTime>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class SelectAreaVideoWidget : public QLabel
{
    Q_OBJECT
public:
    SelectAreaVideoWidget(QWidget* parent=0);
    ~SelectAreaVideoWidget();
    QSize sizeHint() const;

    QRect selection;
    QRect roi; // Region of interest
private:
    QPointF pointToGlobal(int x, int y);

    double zoom;
    QPointF center;
    QImage last_image;
    QImage fix_image;
    QImage last_frame;
    QImage* img;
    bool selecting;
    bool dragging;
    int px;
    int py;
protected:
    void wheelEvent(QWheelEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
signals:
    void newSelection();
    void newArea(QImage);
    void resetRoi();
public slots:
    void selectArea();
    void reset();
    void setFrame(Mat* frame);
    void selectNewArea();
};

#endif // SELECTAREAVIDEOWIDGET_H
