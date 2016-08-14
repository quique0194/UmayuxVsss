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
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class SelectAreaVideoWidget : public QLabel
{
    Q_OBJECT
public:
    SelectAreaVideoWidget();
    SelectAreaVideoWidget(QWidget* parent);
    ~SelectAreaVideoWidget();

    float factor;
    QRect selection;
    QRect roi; // Region of interest
    QImage* area;
private:
    QPointF pointToGlobal(int x, int y);

    double zoom;
    QPointF center;
    QImage last_image;
    QImage fix_image;
    QImage last_frame;
    bool selecting;
protected:
    void wheelEvent(QWheelEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void newSelection();
    void newArea(QImage*);
    void newArea(QPixmap);
    void resetRoi();
public slots:
    void selectArea();
    void reset();
    void setFrame(Mat* frame);
    void selectNewArea();
};

#endif // SELECTAREAVIDEOWIDGET_H
