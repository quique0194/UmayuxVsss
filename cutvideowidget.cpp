#include "cutvideowidget.h"

CutVideoWidget::CutVideoWidget()
{

}

CutVideoWidget::CutVideoWidget(QFrame *frame): QLabel(frame)
{

}

CutVideoWidget::~CutVideoWidget()
{

}

void CutVideoWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        rect.setTopLeft(QPoint(ev->pos().x(), ev->pos().y()));
        rect.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
    }
}

void CutVideoWidget::mouseMoveEvent(QMouseEvent *ev)
{
    rect.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
}

void CutVideoWidget::setFrame(Mat *frame)
{
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(qimg);
    QPainter paint(&pix);
    paint.setPen(Qt::red);
    paint.drawRect(rect);
    setPixmap(pix);
}

