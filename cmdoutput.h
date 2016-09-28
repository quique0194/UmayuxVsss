#ifndef CMDOUTPUT_H
#define CMDOUTPUT_H

#include <algorithm>
#include <QLabel>
#include <QWidget>

using namespace std;

class CmdOutput : public QLabel
{
    Q_OBJECT
public:
    CmdOutput(QWidget* parent=0);
    ~CmdOutput();
public slots:
    void appendText(QString newText);
    void printFinished();
};

#endif // CMDOUTPUT_H
