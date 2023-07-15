#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Command.h"
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QStringList>

class Executor : public QProcess
{
    Q_OBJECT
public:
    Executor(Command *command, bool openTerminal, QString fileNameOutput, QObject *parent = nullptr);
    void start();
    void stop();
    bool isRunning() const { return QProcess::state() != QProcess::NotRunning; }

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus = NormalExit);

signals:
    void executionFinished();

private:
    Command *command;
    bool openTerminal;
    QString fileNameOutput;
    int stopTriggerCount = 0;
};

#endif // EXECUTOR_H
