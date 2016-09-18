#include "asynccmd.h"

AsyncCmd::AsyncCmd(QString _cmd, QStringList _args)
{
    kill_cmd = false;
    start_cmd = false;
    keep_running = true;

    cmd = _cmd;
    args = _args;
}

AsyncCmd::~AsyncCmd()
{

}

void AsyncCmd::run()
{
    while (keep_running) {
        QThread::msleep(100);
        if (start_cmd) {
            QProcess process;
            process.start(cmd, args);
            start_cmd = false;
            emit cmd_started();
            bool finished = false;
            while (!finished) {
                finished = process.waitForFinished(100);
                QString output = process.readAllStandardOutput();
                QString output_error = process.readAllStandardError();
                output.append(output_error);
                emit newText(output);
                if (kill_cmd) {
                    process.kill();
                    kill_cmd = false;
                }
            }
            emit cmd_finished();
        }
    }
    cout << "End of command" << endl;
}

void AsyncCmd::run_cmd()
{
    start_cmd = true;
}

void AsyncCmd::terminate_thread()
{
    kill_cmd = true;
    keep_running = false;
}

void AsyncCmd::terminate_cmd()
{
    kill_cmd = true;
}
