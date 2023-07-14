#include "Command.h"

Command::Command(QString command) : QObject()
{
    state = WAITING;
    this->command = command;
}

Command::Command(const Command &other)
{
    command = other.command;
    state = other.state;
    start = other.start;
    end = other.end;
}

Command &Command::operator=(const Command &other)
{
    command = other.command;
    state = other.state;
    start = other.start;
    end = other.end;
    return *this;
}

bool Command::setCommand(QString command)
{
    if(state != WAITING) {
        qWarning() << "Tryed to modify a command already executed";
        return false;
    }
    this->command = command;
    return true;
}

QString Command::getStartEndTimeString() const
{
    if(state == WAITING)
        return "";

    QString startStr = start.time().toString();
    QString endStr = end.time().toString();
    if(state == EXECUTING)
        endStr = QString::number(start.secsTo(QDateTime::currentDateTime())) + "s";

    return startStr + " - " + endStr;
}
