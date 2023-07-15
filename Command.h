#ifndef COMMAND_H
#define COMMAND_H

#include <QDateTime>

class Command : public QObject
{
    Q_OBJECT
public:
    enum CommandState {
        WAITING,
        EXECUTING,
        EXECUTED,
        FAILED
    };
    Q_ENUM(CommandState)

    Command(QString command);
    Command(const Command &other);

    Command &operator=(const Command &other);

    QString getCommand() const { return command; }
    bool setCommand(QString command);

    CommandState getState() const { return state; }
    bool isWaiting() const { return state == WAITING; }
    bool isExecuting() const { return state == EXECUTING; }
    bool isExecuted() const { return (state == EXECUTED) | (state == FAILED); }
    void setState(CommandState state);

    QDateTime getStartTime() const { return start; }
    QDateTime getEndTime() const { return isExecuted() ? end : QDateTime::currentDateTime(); }
    QString getStartEndTimeString() const;

    int getExitCode() const { return exitCode; }
    bool hasCrash() const { return exitCrash; }
    void setExitCode(int exitCode, bool exitCrash);

signals:
    void changed();

private:
    QString command;
    CommandState state;
    QDateTime start;
    QDateTime end;

    int exitCode = 0;
    bool exitCrash = false;
};

#endif // COMMAND_H
