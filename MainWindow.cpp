#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listCmds->setModel(&commands);
    ui->listCmds->setColumnWidth(0, 75);
    ui->listCmds->setColumnWidth(1, 110);
    ui->listCmds->setColumnWidth(2, 367);

    commands.addCommand(Command("firstcmd"));
    commands.addCommand(Command("2ndcmd"));

    connect(ui->bCmdAdd, &QPushButton::clicked, this, &MainWindow::onCmdAddClicked);
    connect(ui->bCmdInsert, &QPushButton::clicked, this, &MainWindow::onCmdInsertClicked);
    connect(ui->bCmdDel, &QPushButton::clicked, this, &MainWindow::onCmdDelClicked);
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
    commands.addCommand(Command(cmd));
    ui->leCmd->setText("");
}

void MainWindow::onCmdInsertClicked()
{
    QString cmd = ui->leCmd->text();
    ui->leCmd->setText("");

    int indexFirstWaiting = commands.getNextExecutingCommandIndex();
    int lastSelectedIndex = getLastSelectedRow();

    if(lastSelectedIndex == -1) {
        commands.addCommand(Command(cmd));
    }
    else {
        int index = qMax(lastSelectedIndex + 1, indexFirstWaiting);
        commands.insertCommand(Command(cmd), index);
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
