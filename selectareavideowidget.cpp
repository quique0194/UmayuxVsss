#include "selectareavideowidget.h"

SelectAreaVideoWidget::SelectAreaVideoWidget(): zoom(1.0), center(QPointF(0.5, 0.5))
{

}

SelectAreaVideoWidget::SelectAreaVideoWidget(QWidget *parent): QLabel(parent), zoom(1.0), center(QPointF(0.5, 0.5))
{
}


SelectAreaVideoWidget::~SelectAreaVideoWidget()
{

}

void SelectAreaVideoWidget::wheelEvent(QWheelEvent * ev)
{
    if (fix_image.isNull()) {
        fix_image = last_image;
    }
    int x = ev->pos().x();
    int y = ev->pos().y();
    QPointF click((double)x/last_image.size().width(), (double)y/last_image.size().height());
    QPointF delta = click - QPointF(0.5, 0.5);
    delta *= (1.0/zoom);
    click = center + delta;
    delta = QPointF(0.5, 0.5) - click;
    if (ev->angleDelta().y() > 0) {
        zoom *= 1.25;
    }
    else {
        zoom *= 0.8;
    }
    zoom = max(zoom, 1.0);

    delta *= (1.0/zoom);
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

void SelectAreaVideoWidget::setFrame(Mat *frame)
{
    if (fix_image.isNull()) {
        QImage qimg_from_frame((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
        factor = max((float)frame->cols/baseSize().width(), (float)frame->rows/baseSize().height());
        QImage qimg = qimg_from_frame.copy();
        if (qimg.isNull()) {
            return;
        }
        qimg = qimg.scaled(baseSize(), Qt::KeepAspectRatio);
        last_image = qimg;
        resize(qimg.size());
        QPixmap pix = QPixmap::fromImage(qimg);
    //    QPainter paint(&pix);
    //    paint.setPen(Qt::red);
    //    if (selection.isValid()) {
    //        paint.drawRect(selection);
    //    }
        setPixmap(pix);
    }
    else {
        QPixmap pix = QPixmap::fromImage(fix_image);
        resize(fix_image.size());
        setPixmap(pix);
    }
}

void SelectAreaVideoWidget::selectNewArea()
{
    fix_image = QImage();
}
