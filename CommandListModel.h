#ifndef COMMANDLISTMODEL_H
#define COMMANDLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "Command.h"
#include <QFont>
#include <QItemSelectionModel>

#define COLS 3

class CommandListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CommandListModel(QObject *parent = nullptr);

    void addCommand(Command command);
    void insertCommand(Command command, int index);
    void deleteCommand(int index);
    void deleteCommands(const QItemSelectionModel *select);
    Command *getCommand(const QModelIndex &index);
    const Command *getCommandConst(const QModelIndex &index) const;
    int getNextExecutingCommandIndex() const;

    int size() const { return commands.size(); }

    // override QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<Command> commands;
};

#endif // COMMANDLISTMODEL_H
