#ifndef SELECTAREAVIDEOWIDGET_H
#define SELECTAREAVIDEOWIDGET_H

#include <QLabel>
#include <QFrame>
#include <opencv2/opencv.hpp>

using namespace cv;


class SelectAreaVideoWidget : public QLabel
{
    Q_OBJECT
public:
    SelectAreaVideoWidget();
    SelectAreaVideoWidget(QFrame* frame);
    ~SelectAreaVideoWidget();

    float factor;
public slots:
    void setFrame(Mat* frame);
};

#endif // SELECTAREAVIDEOWIDGET_H
