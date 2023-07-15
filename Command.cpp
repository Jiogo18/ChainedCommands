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

void Command::setState(CommandState state)
{
    switch(state) {
    case WAITING:
        if(this->state != WAITING)
            throw "Command can't go from executed to waiting";
        break;
    case EXECUTING:
        if(this->state == WAITING)
            start = QDateTime::currentDateTime();
        else if(this->state == EXECUTING)
            return;
        else
            throw "Command already executed can't be reexecuted";
        break;
    case EXECUTED:
    case FAILED:
        if(this->state == WAITING)
            start = end = QDateTime::currentDateTime();
        else if(this->state == EXECUTING)
            end = QDateTime::currentDateTime();
        else
            return;
        break;
    }

    if(this->state != state) {
        this->state = state;
        emit changed();
    }
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

void Command::setExitCode(int exitCode, bool exitCrash) {
    this->exitCode = exitCode;
    this->exitCrash = exitCrash;
    emit changed();
}
