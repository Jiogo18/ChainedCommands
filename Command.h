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
        EXECUTED
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
    bool isExecuted() const { return state == EXECUTED; }

    QDateTime getStartTime() const { return start; }
    QDateTime getEndTime() const { return end; }
    QString getStartEndTimeString() const;

private:
    QString command;
    CommandState state;
    QDateTime start;
    QDateTime end;
};

#endif // COMMAND_H
