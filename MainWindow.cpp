#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    chainedExecutor(&commands, this)
{
    ui->setupUi(this);

    ui->listCmds->setModel(&commands);
    ui->listCmds->setColumnWidth(0, 75);
    ui->listCmds->setColumnWidth(1, 110);
    ui->listCmds->setColumnWidth(2, 367);

    commands.addCommand(new Command("echo a"));
    commands.addCommand(new Command("echo b"));
    commands.addCommand(new Command("timeout 5"));
    commands.addCommand(new Command("echo c"));
    commands.addCommand(new Command("sleep 3"));

    connect(ui->bCmdAdd, &QPushButton::clicked, this, &MainWindow::onCmdAddClicked);
    connect(ui->bCmdInsert, &QPushButton::clicked, this, &MainWindow::onCmdInsertClicked);
    connect(ui->bCmdDel, &QPushButton::clicked, this, &MainWindow::onCmdDelClicked);

    connect(ui->cbExecuteInTerminal, &QCheckBox::clicked, &chainedExecutor, &ChainedExecutor::setOpenTerminal);
    connect(ui->cbOutputInFile, &QCheckBox::clicked, &chainedExecutor, &ChainedExecutor::setOutputInFile);
    chainedExecutor.setOpenTerminal(ui->cbExecuteInTerminal->isChecked());
    chainedExecutor.setOutputInFile(ui->cbOutputInFile->isChecked());

    connect(ui->bExecuteNext, &QPushButton::clicked, &chainedExecutor, &ChainedExecutor::executeNext);
    connect(ui->bStop, &QPushButton::clicked, &chainedExecutor, &ChainedExecutor::stopCurrentAndPause);
    connect(ui->bStopCurrentAndContinue, &QPushButton::clicked, &chainedExecutor, &ChainedExecutor::stopCurrentAndContinue);
    connect(ui->bStopAfterCurrent, &QPushButton::clicked, &chainedExecutor, &ChainedExecutor::stopAfterCurrent);

    connect(ui->actionExecuteNext, &QAction::triggered, &chainedExecutor, &ChainedExecutor::executeNext);
    connect(ui->actionStop, &QAction::triggered, &chainedExecutor, &ChainedExecutor::stopCurrentAndPause);
    connect(ui->actionStopCurrentAndContinue, &QAction::triggered, &chainedExecutor, &ChainedExecutor::stopCurrentAndContinue);
    connect(ui->actionStopAfterCurrent, &QAction::triggered, &chainedExecutor, &ChainedExecutor::stopAfterCurrent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getLastSelectedRow() const
{
    QItemSelectionModel *select = ui->listCmds->selectionModel();
    if(select->hasSelection()) {
        QModelIndexList selection = select->selectedIndexes();
        int lastSelectedRow = -1;
        for(int i = 0; i < selection.size(); i++) {
            int index = selection.at(i).row();
            if(lastSelectedRow < index)
                lastSelectedRow = index;
        }
        return lastSelectedRow;
    }
    else {
        return -1;
    }
}

void MainWindow::onCmdAddClicked()
{
    QString cmd = ui->leCmd->text();
    if(cmd.isEmpty()) return;
    ui->leCmd->setText("");
    commands.addCommand(new Command(cmd));
}

void MainWindow::onCmdInsertClicked()
{
    QString cmd = ui->leCmd->text();
    if(cmd.isEmpty()) return;
    ui->leCmd->setText("");

    int indexFirstWaiting = commands.getNextExecutingCommandIndex();
    int lastSelectedIndex = getLastSelectedRow();

    if(lastSelectedIndex == -1) {
        commands.addCommand(new Command(cmd));
    }
    else {
        int index = qMax(lastSelectedIndex + 1, indexFirstWaiting);
        commands.insertCommand(new Command(cmd), index);
    }
}

void MainWindow::onCmdDelClicked()
{
    commands.deleteCommands(ui->listCmds->selectionModel());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key::Key_Delete:
        onCmdDelClicked();
        break;
    case Qt::Key::Key_Return:
        onCmdAddClicked();
        break;
    }
}
