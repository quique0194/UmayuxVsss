#include "showinrangewidget.h"

ShowInRangeWidget::ShowInRangeWidget(QWidget* parent): QLabel(parent), calib(NULL)
{

}

ShowInRangeWidget::~ShowInRangeWidget()
{

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
    Mat binarized;
    inRange(*frame, Scalar(calib->data[0], calib->data[1], calib->data[2]),
            Scalar(calib->data[3], calib->data[4], calib->data[5]), binarized);
    cvtColor(binarized, binarized, CV_GRAY2RGB);
    QImage qimg((uchar*)binarized.data, binarized.cols, binarized.rows, binarized.step, QImage::Format_RGB888);
    qimg = qimg.scaled(baseSize(), Qt::KeepAspectRatio);
    QPixmap pix = QPixmap::fromImage(qimg);
    setPixmap(pix);
}
