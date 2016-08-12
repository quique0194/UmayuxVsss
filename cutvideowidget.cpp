#include "cutvideowidget.h"

CutVideoWidget::CutVideoWidget(): selecting(false)
{

}

CutVideoWidget::CutVideoWidget(QFrame *frame): QLabel(frame), selecting(false)
{

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

void CutVideoWidget::setFrame(Mat *frame)
{
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(qimg);
    if (pix.isNull()) {
        return;
    }
    QPainter paint(&pix);
    paint.setPen(Qt::red);
    paint.drawRect(selection);
    setPixmap(pix);
}

void CutVideoWidget::cut()
{
    roi = selection;
    selection = QRect();
    emit newRoi(&roi);
}

void CutVideoWidget::reset()
{
    selection = QRect();
    roi = QRect();
    emit resetRoi();
}

