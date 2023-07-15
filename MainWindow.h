#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "CommandListModel.h"
#include "ChainedExecutor.h"
#include <QtMath>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int getLastSelectedRow() const;


private slots:
    void onCmdAddClicked();
    void onCmdInsertClicked();
    void onCmdDelClicked();

    void keyPressEvent(QKeyEvent  *event) override;


private:
    Ui::MainWindow *ui;
    CommandListModel commands;
    ChainedExecutor chainedExecutor;
};

#endif // MAINWINDOW_H
