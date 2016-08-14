#include "showimagewidget.h"

ShowImageWidget::ShowImageWidget(QWidget *parent): QLabel(parent)
{

}

ShowImageWidget::~ShowImageWidget()
{

}

void ShowImageWidget::setImage(QImage img)
{
    img = img.scaled(baseSize(), Qt::KeepAspectRatio);
    QPixmap pix = QPixmap::fromImage(img);
    setPixmap(pix);
}

