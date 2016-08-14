#include "selectareavideowidget.h"

SelectAreaVideoWidget::SelectAreaVideoWidget(): zoom(1.0), center(QPointF(0.5, 0.5)), selecting(false)
{

}

SelectAreaVideoWidget::SelectAreaVideoWidget(QWidget *parent): QLabel(parent), zoom(1.0), center(QPointF(0.5, 0.5)), selecting(false)
{
}


SelectAreaVideoWidget::~SelectAreaVideoWidget()
{

}

QPointF SelectAreaVideoWidget::pointToGlobal(int x, int y) {
    QPointF click((double)x/size().width(), (double)y/size().height());
    QPointF delta = click - QPointF(0.5, 0.5);
    delta *= (1.0/zoom);
    click = center + delta;
    return click;
}

void SelectAreaVideoWidget::wheelEvent(QWheelEvent * ev)
{
    if (fix_image.isNull()) {
        fix_image = last_image;
    }
    int x = ev->pos().x();
    int y = ev->pos().y();

    QPointF click = pointToGlobal(x, y);
    QPointF delta = center - click;
    float zoom_delta;
    if (ev->angleDelta().y() > 0) {
        zoom_delta = 1.25;
    }
    else {
        zoom_delta = 0.8;
    }
    zoom *= zoom_delta;
    if (zoom < 1) {
        zoom = 1;
        zoom_delta = 1;
    }

    delta /= zoom_delta;
    center = click + delta;

    QSize s = 0.5*(1.0/zoom)*last_image.size();
    QRect rect;
    rect.setCoords(round(center.x()*last_image.size().width() - s.width()),
                   round(center.y()*last_image.size().height() - s.height()),
                   round(center.x()*last_image.size().width() + s.width()),
                   round(center.y()*last_image.size().height() + s.height()));
    fix_image = last_image.copy(rect);
    fix_image = fix_image.scaled(baseSize(), Qt::KeepAspectRatio);
}


void SelectAreaVideoWidget::mousePressEvent(QMouseEvent *ev)
{
    if (roi.isNull() && ev->button() == Qt::LeftButton) {
        selection.setTopLeft(QPoint(ev->pos().x(), ev->pos().y()));
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
        selecting = true;
    }
}

void SelectAreaVideoWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (roi.isNull() && selecting) {
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
    }
}

void SelectAreaVideoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (roi.isNull() && ev->button() == Qt::LeftButton) {
        selecting = false;
        emit newSelection();
    }
}

void SelectAreaVideoWidget::selectArea()
{
    roi = selection;
    selection = QRect();
    int x1, y1, x2, y2;
    roi.getCoords(&x1, &y1, &x2, &y2);
    QPointF tl = pointToGlobal(x1, y1);
    QPointF br = pointToGlobal(x2, y2);
    int w = last_frame.size().width();
    int h = last_frame.size().height();
    roi.setCoords(tl.x()*w, tl.y()*h, br.x()*w, br.y()*h);
    QImage my_area = last_frame.copy(roi);
    roi = QRect();
    emit newArea(my_area);
}

void SelectAreaVideoWidget::reset()
{
    selection = QRect();
    roi = QRect();
    emit resetRoi();
}

void SelectAreaVideoWidget::setFrame(Mat *frame)
{
    QPixmap pix;
    if (fix_image.isNull()) {
        QImage qimg_from_frame((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
        if (qimg_from_frame.isNull()) {
            return;
        }
        factor = max((float)frame->cols/baseSize().width(), (float)frame->rows/baseSize().height());
        QImage qimg = qimg_from_frame.copy();
        qimg = qimg.scaled(baseSize(), Qt::KeepAspectRatio);
        last_frame = qimg_from_frame.copy();
        last_image = qimg;
        pix = QPixmap::fromImage(qimg);
    }
    else {
        pix = QPixmap::fromImage(fix_image);
    }
    QPainter paint(&pix);
    paint.setPen(Qt::red);
    if (selection.isValid()) {
        paint.drawRect(selection);
    }
    setPixmap(pix);
}

void SelectAreaVideoWidget::selectNewArea()
{
    fix_image = QImage();
}
