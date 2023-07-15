#include "Executor.h"

Executor::Executor(Command *command, bool openTerminal, QString fileNameOutput, QObject *parent)
    : QProcess(parent), command(command), openTerminal(openTerminal), fileNameOutput(fileNameOutput)
{
    connect(this, &QProcess::finished, this, &Executor::onFinished);
}

void Executor::start()
{
    QString cmdStr = command->getCommand();

    if(!fileNameOutput.isEmpty()) {
        if(fileNameOutput.contains("%TYPE%")) {
            QDir dir(fileNameOutput.left(fileNameOutput.lastIndexOf("/")));
            dir.mkpath(dir.absolutePath());
            QFile fileCommand(QString(fileNameOutput).replace("%TYPE%", "CMD"));

            fileCommand.open(QFile::WriteOnly);
            fileCommand.write(cmdStr.toUtf8());
            fileCommand.close();
            QProcess::setStandardErrorFile(QString(fileNameOutput).replace("%TYPE%", "ERR"));
        }
        QProcess::setStandardOutputFile(QString(fileNameOutput).replace("%TYPE%", "OUT"));
    }
    else {
        QProcess::setStandardOutputFile(nullDevice());
        QProcess::setStandardErrorFile(nullDevice());
    }
    QProcess::setStandardInputFile(nullDevice());

    if(openTerminal) {
        // QProcess::start("cmd.exe", QStringList({ "/C", "start /WAIT cmd.exe /C", cmdStr }));
        QProcess::start("cmd.exe", QStringList({ "/C", cmdStr }));
    }
    else {
        QProcess::startCommand(cmdStr);
        // echo doesn't work with startCommand
        // timeout never works
        // sleep works with pwsh.exe
        // I recommand you to use pwsh/cmd as you want directly in the command
    }
    command->setState(Command::EXECUTING);

    if(!QProcess::waitForStarted()) {
        qDebug() << "Command failed to start" << QProcess::errorString() << QProcess::error() << QProcess::exitCode() << QProcess::exitStatus();
        command->setState(Command::FAILED);
        emit executionFinished();
    }
}

void Executor::stop()
{
    stopTriggerCount++;
    if(stopTriggerCount <= 3) {
        QProcess::terminate(); // SIGTERM/WM_CLOSE, may not exit
    }
    else {
        QProcess::kill();// SIGKILL, exit immediately
    }
}

void Executor::onFinished(int exitCode, ExitStatus exitStatus)
{
    qDebug() << "Execution finished with code" << exitCode << "and status" << exitStatus;
    command->setState(exitStatus == CrashExit ? Command::FAILED : Command::EXECUTED);
    command->setExitCode(exitCode, exitStatus == CrashExit);
    emit executionFinished();
}
