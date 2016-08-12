#ifndef EASYDISABLEBTN_H
#define EASYDISABLEBTN_H

#include <QFrame>
#include <QPushButton>

class EasyDisableBtn : public QPushButton
{
    Q_OBJECT
public:
    EasyDisableBtn();
    EasyDisableBtn(QFrame* frame);
    ~EasyDisableBtn();
public slots:
    void disable();
    void enable();
};

#endif // EASYDISABLEBTN_H
