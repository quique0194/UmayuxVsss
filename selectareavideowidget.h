#ifndef SELECTAREAVIDEOWIDGET_H
#define SELECTAREAVIDEOWIDGET_H

#include <cmath>
#include <algorithm>
#include <QLabel>
#include <QFrame>
#include <QWheelEvent>
#include <QImage>
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
private:
    double zoom;
    QPointF center;
    QImage last_image;
    QImage fix_image;
protected:
    void wheelEvent(QWheelEvent *ev);
public slots:
    void setFrame(Mat* frame);
    void selectNewArea();
};

#endif // SELECTAREAVIDEOWIDGET_H
