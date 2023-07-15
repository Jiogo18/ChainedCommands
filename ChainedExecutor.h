#ifndef CHAINEDEXECUTOR_H
#define CHAINEDEXECUTOR_H

#include "CommandListModel.h"
#include "Executor.h"

class ChainedExecutor : public QObject
{
    Q_OBJECT
public:
    ChainedExecutor(CommandListModel *commands, QObject *parent = nullptr);
    ~ChainedExecutor();

    bool executeNext();
    void stopCurrentAndPause();
    void stopCurrentAndContinue();
    void stopAfterCurrent();

    bool isOpenTerminal() const { return openTerminal; }
    bool isOutputInFile() const { return outputInFile; }
    void setOpenTerminal(bool openTerminal) { this->openTerminal = openTerminal; }
    void setOutputInFile(bool outputInFile) { this->outputInFile = outputInFile; }

private slots:
    void onExecutorFinished();

private:
    CommandListModel *commands;
    Executor *currentExecutor;
    bool pause;
    bool openTerminal = true;
    bool outputInFile = true;
};

#endif // CHAINEDEXECUTOR_H
