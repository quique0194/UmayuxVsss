#include "easydisablebtn.h"

EasyDisableBtn::EasyDisableBtn()
{

}

EasyDisableBtn::EasyDisableBtn(QFrame* frame): QPushButton(frame)
{

}

EasyDisableBtn::~EasyDisableBtn()
{

}

void EasyDisableBtn::disable()
{
    setEnabled(false);
}

void EasyDisableBtn::enable()
{
    setEnabled(true);
}
