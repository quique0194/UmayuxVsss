#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QPixmap>

class ShowImageWidget : public QLabel
{
    Q_OBJECT
public:
    ShowImageWidget(QWidget* parent=0);
    ~ShowImageWidget();
public slots:
    void setImage(QImage img);
};

#endif // SHOWIMAGEWIDGET_H
