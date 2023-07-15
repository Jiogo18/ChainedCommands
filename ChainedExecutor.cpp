#include "ChainedExecutor.h"

ChainedExecutor::ChainedExecutor(CommandListModel *commands, QObject *parent) : QObject(parent)
{
    this->commands = commands;
    currentExecutor = nullptr;
    pause = true;
}

ChainedExecutor::~ChainedExecutor()
{
    if(currentExecutor != nullptr) {
        delete currentExecutor;
        currentExecutor = nullptr;
    }
}

bool ChainedExecutor::executeNext()
{
    if(currentExecutor != nullptr) {
        if(currentExecutor->isRunning())
            return false;
        delete currentExecutor;
    }

    pause = false;

    int nextIndex = commands->getNextExecutingCommandIndex();
    Command *nextCmd = commands->getCommand(nextIndex);
    if(nextCmd == nullptr)
        return false;
    nextCmd->setState(Command::EXECUTING);

    QDateTime startTime = nextCmd->getStartTime();
    QString outputFileName;
    if(outputInFile) {
        outputFileName = "output/exec_" + startTime.toString("yyyyMMdd_HHmmss/zzz") + "_%TYPE%.txt";
    }

    currentExecutor = new Executor(nextCmd, openTerminal, outputFileName, this);

    connect(currentExecutor, &Executor::executionFinished, this, &ChainedExecutor::onExecutorFinished);

    currentExecutor->start();
    return true;
}

void ChainedExecutor::stopCurrentAndPause()
{
    pause = true;
    stopCurrentAndContinue();
}

void ChainedExecutor::stopCurrentAndContinue()
{
    if(currentExecutor != nullptr) {
        currentExecutor->stop();
    }
}

void ChainedExecutor::stopAfterCurrent()
{
    pause = true;
}

void ChainedExecutor::onExecutorFinished()
{
    if(currentExecutor != nullptr) {
        delete currentExecutor;
        currentExecutor = nullptr;
    }

    if(!pause) {
        executeNext();
    }
}
