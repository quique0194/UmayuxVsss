#include "cmdoutput.h"

CmdOutput::CmdOutput(QWidget* parent): QLabel(parent)
{

}

CmdOutput::~CmdOutput()
{

}

void CmdOutput::appendText(QString newText)
{
    QString prev = text();
    prev.append(newText);
    prev.remove(0, max(0, prev.size()-2000));
    setText(prev);
}

void CmdOutput::printFinished()
{
    appendText("\nFinished\n");
}

