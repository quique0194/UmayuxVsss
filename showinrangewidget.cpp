#include "showinrangewidget.h"

ShowInRangeWidget::ShowInRangeWidget(QWidget* parent): QLabel(parent), calib(NULL), img(NULL)
{

}

ShowInRangeWidget::~ShowInRangeWidget()
{
    if (img) {
        delete img;
    }
}

void ShowInRangeWidget::paintEvent(QPaintEvent*)
{
    if (img) {
        QPainter paint(this);
        paint.drawImage(img->rect(), *img, img->rect());
    }
}

QSize ShowInRangeWidget::sizeHint() const
{
    if (img) {
        return QSize(img->width(), img->height());
    } else {
        return QWidget::sizeHint();
    }
}

void ShowInRangeWidget::newCalibration(Calibration *c)
{
    calib = c;
}

void ShowInRangeWidget::setFrame(Mat *frame)
{
    if (!isVisible() || calib == NULL) {
        return;
    }
    Mat binarized = *frame;
    if (calib->color != "red") {
        cvtColor(binarized, binarized, CV_BGR2HSV);
    }
    inRange(binarized, Scalar(calib->data[0], calib->data[1], calib->data[2]),
            Scalar(calib->data[3], calib->data[4], calib->data[5]), binarized);
    cvtColor(binarized, binarized, CV_GRAY2RGB);
    QImage qimg((uchar*)binarized.data, binarized.cols, binarized.rows, binarized.step, QImage::Format_RGB888);
    qimg = qimg.scaled(baseSize(), Qt::KeepAspectRatio);
    if (img) {
        delete img;
    }
    img = new QImage(qimg);
    repaint();
    if (width() != img->width()) {
        resize(img->width(), img->height());
        updateGeometry();
    }
}
