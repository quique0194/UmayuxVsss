#include "selectareavideowidget.h"

SelectAreaVideoWidget::SelectAreaVideoWidget(QWidget *parent): QLabel(parent),
    zoom(1.0), center(QPointF(0.5, 0.5)), selecting(false), dragging(false)
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
    if (ev->button() == Qt::LeftButton) {
        if (fix_image.isNull()) {
            fix_image = last_image;
        }
        selecting = true;
        selection.setTopLeft(QPoint(ev->pos().x(), ev->pos().y()));
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
    }
    else if (ev->button() == Qt::RightButton) {
        if (fix_image.isNull()) {
            fix_image = last_image;
        }
        dragging = true;
        px = -1;
        py = -1;
    }
}

void SelectAreaVideoWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (selecting) {
        selection.setBottomRight(QPoint(ev->pos().x(), ev->pos().y()));
    }
    else if (dragging) {
        float dx, dy;
        if (px == -1) {
            dx = 0;
            dy = 0;
        } else {
            dx = ev->pos().x() - px;
            dy = ev->pos().y() - py;
        }
        px = ev->pos().x();
        py = ev->pos().y();

        QPointF dg(dx/size().width(), dy/size().height());
        dg *= (1.0/zoom);
        center = center - dg;

        QSize s = 0.5*(1.0/zoom)*last_image.size();
        QRect rect;
        rect.setCoords(round(center.x()*last_image.size().width() - s.width()),
                       round(center.y()*last_image.size().height() - s.height()),
                       round(center.x()*last_image.size().width() + s.width()),
                       round(center.y()*last_image.size().height() + s.height()));
        fix_image = last_image.copy(rect);
        fix_image = fix_image.scaled(baseSize(), Qt::KeepAspectRatio);
    }
    else if (!dragging) {
        px = -1;
        py = -1;
    }

}

void SelectAreaVideoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        selecting = false;
        emit newSelection();
    }
    else if (dragging) {
        dragging = false;
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
    roi.setCoords(ceil(tl.x()*w), ceil(tl.y()*h), floor(br.x()*w), floor(br.y()*h));
    QImage my_area = last_frame.copy(roi);
    roi = QRect();
    emit newArea(my_area);
}

void SelectAreaVideoWidget::reset()
{
    fix_image = QImage();
    selection = QRect();
    center = QPointF(0.5, 0.5);
    zoom = 1;
}

void SelectAreaVideoWidget::setFrame(Mat *frame)
{
    QPixmap pix;
    if (fix_image.isNull()) {
        QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
        qimg = qimg.copy();
        last_frame = qimg;
        QTime time;
        time.start();
        QImage qimg2 = qimg.scaled(baseSize(), Qt::KeepAspectRatio);
        cout << "TIME " << time.elapsed() << endl;
        last_image = qimg2;
        pix = QPixmap::fromImage(qimg2);
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
