#ifndef ASYNCCMD_H
#define ASYNCCMD_H

#include <iostream>
#include <QtCore>
#include <QLabel>
#include <QPushButton>
#include <QThread>

using namespace std;

class AsyncCmd : public QThread
{
    Q_OBJECT
public:
    AsyncCmd(QString cmd, QStringList args);
    ~AsyncCmd();
private:
    void run();

    bool kill_cmd;
    bool start_cmd;
    bool keep_running;

    QString cmd;
    QStringList args;
signals:
    void newText(QString output);
    void cmd_started();
    void cmd_finished();
public slots:
    void run_cmd();
    void terminate_cmd();
    void terminate_thread();
};

#endif // ASYNCCMD_H
