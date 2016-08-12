#include "selectareavideowidget.h"

SelectAreaVideoWidget::SelectAreaVideoWidget()
{

}

SelectAreaVideoWidget::SelectAreaVideoWidget(QFrame *frame): QLabel(frame)
{

}

SelectAreaVideoWidget::~SelectAreaVideoWidget()
{

}

void SelectAreaVideoWidget::setFrame(Mat *frame)
{
    QImage qimg_from_frame((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    factor = max((float)frame->cols/baseSize().width(), (float)frame->rows/baseSize().height());
    QImage qimg = qimg_from_frame.copy();
    if (qimg.isNull()) {
        return;
    }
    qimg = qimg.scaled(baseSize().width(), baseSize().height(), Qt::KeepAspectRatio);
    setMaximumSize(qimg.size());
    setMinimumSize(qimg.size());
    QPixmap pix = QPixmap::fromImage(qimg);
//    QPainter paint(&pix);
//    paint.setPen(Qt::red);
//    if (selection.isValid()) {
//        paint.drawRect(selection);
//    }
    setPixmap(pix);
}
