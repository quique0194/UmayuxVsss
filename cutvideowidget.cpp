#include "cutvideowidget.h"

CutVideoWidget::CutVideoWidget(QWidget *frame): QLabel(frame), selecting(false)
{
    img = 0;
}

CutVideoWidget::~CutVideoWidget()
{

}

QSize CutVideoWidget::sizeHint() const
{
    if (img) {
        return QSize(img->width(), img->height());
    } else {
        return QWidget::sizeHint();
    }
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

void CutVideoWidget::paintEvent(QPaintEvent *)
{
    if (img) {
        QPainter paint(this);
        paint.drawImage(img->rect(), *img, img->rect());
        paint.setPen(Qt::red);
        if (selection.isValid()) {
            paint.drawRect(selection);
        }
    }
}

void CutVideoWidget::setFrame(Mat *frame)
{
    if (!isVisible()) {
        return;
    }
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    qimg = qimg.copy();
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

