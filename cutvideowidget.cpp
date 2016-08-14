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
    QImage qimg_from_frame((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    if (qimg_from_frame.isNull()) {
        return;
    }
    factor = max((float)frame->cols/baseSize().width(), (float)frame->rows/baseSize().height());
    QImage qimg = qimg_from_frame.copy();
    qimg = qimg.scaled(baseSize().width(), baseSize().height(), Qt::KeepAspectRatio);
    setMaximumSize(qimg.size());
    setMinimumSize(qimg.size());
    QPixmap pix = QPixmap::fromImage(qimg);
    QPainter paint(&pix);
    paint.setPen(Qt::red);
    if (selection.isValid()) {
        paint.drawRect(selection);
    }
    setPixmap(pix);
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

