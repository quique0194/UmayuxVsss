#include "cutvideowidget.h"

CutVideoWidget::CutVideoWidget(QFrame *frame): QLabel(frame), selecting(false)
{
    img = 0;
}

CutVideoWidget::~CutVideoWidget()
{

}

void CutVideoWidget::mousePressEvent(QMouseEvent *ev)
{
    if (roi.isNull() && ev->button() == Qt::LeftButton) {
        selection.setTopLeft(QPoint(ev->pos().x(), ev->pos().y()));
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
        selecting = true;
    }
}

void CutVideoWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (roi.isNull() && selecting) {
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
    }
}

void CutVideoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (roi.isNull() && ev->button() == Qt::LeftButton) {
        selecting = false;
        emit newSelection();
    }
}

void CutVideoWidget::paintEvent(QPaintEvent *ev)
{
    if (img) {
        QPainter paint(this);
        paint.drawImage(rect(), *img, img->rect());
        paint.setPen(Qt::red);
        if (selection.isValid()) {
            paint.drawRect(selection);
        }
    }
}

void CutVideoWidget::setFrame(Mat *frame)
{
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    qimg = qimg.copy();
    factor = max((float)frame->cols/width(), (float)frame->rows/height());
    if (img) {
        delete img;
    }
    img = new QImage(qimg);
    repaint();
}

void CutVideoWidget::cut()
{
    roi = selection;
    selection = QRect();
    int x1, y1, x2, y2;
    roi.getCoords(&x1, &y1, &x2, &y2);
    roi.setCoords(x1*factor, y1*factor, x2*factor, y2*factor);
    emit newRoi(&roi);
}

void CutVideoWidget::reset()
{
    selection = QRect();
    roi = QRect();
    emit resetRoi();
}

