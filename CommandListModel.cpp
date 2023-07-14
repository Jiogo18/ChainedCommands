#include "CommandListModel.h"

CommandListModel::CommandListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void CommandListModel::addCommand(Command command)
{
    beginInsertRows(QModelIndex(), commands.size() - 1, commands.size() - 1);
    commands.append(command);
    endInsertRows();
}

void CommandListModel::insertCommand(Command command, int index)
{
    beginInsertRows(QModelIndex(), index, index);
    commands.insert(index, command);
    endInsertRows();
}

void CommandListModel::deleteCommand(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    commands.remove(index);
    endRemoveRows();
}

void CommandListModel::deleteCommands(const QItemSelectionModel *select)
{
    if(!select->hasSelection())
        return;
    QList<int> commandsIndex;
    QModelIndexList selection = select->selectedIndexes();
    for(int i = 0; i < selection.size(); i++) {
        int row = selection.at(i).row();
        if(commandsIndex.contains(row))
            continue;

        const Command *cmd = getCommandConst(selection.at(i));
        if(cmd == nullptr)
            continue;

        if(cmd->isExecuting())
            continue; // can't remove now

        commandsIndex.append(row);
    }

    std::sort(commandsIndex.begin(), commandsIndex.end());
    for(int to = selection.size() - 1; to >= 0; to--) {
        // Remove a block of elements at [fromIndex;toIndex]
        int toIndex = commandsIndex.at(to);

        int from = to;
        while(from > 0 && commandsIndex.at(from - 1) == toIndex)
            from--;
        int fromIndex = commandsIndex.at(from);


        beginRemoveRows(QModelIndex(), fromIndex, toIndex);
        commands.remove(fromIndex, toIndex - fromIndex + 1);
        endRemoveRows();

        to = from;
    }
}

Command *CommandListModel::getCommand(const QModelIndex &index)
{
    if(index.row() >= commands.size()) return nullptr;
    return &commands[index.row()];
}

const Command *CommandListModel::getCommandConst(const QModelIndex &index) const
{
    if(index.row() >= commands.size()) return nullptr;
    return &commands.at(index.row());
}

int CommandListModel::getNextExecutingCommandIndex() const
{
    for(int i = 0; i < commands.size(); i++) {
        if(commands.at(i).isWaiting()) return i;
    }
    return commands.size();
}

int CommandListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return commands.size();
}

int CommandListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COLS;
}

QVariant CommandListModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();

    const Command *cmd = getCommandConst(index);
    if(cmd == nullptr) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if(col == 0) return QVariant::fromValue(cmd->getState()).toString();
        if(col == 1) return cmd->getStartEndTimeString();
        if(col == 2) return cmd->getCommand();
        break;
    case Qt::FontRole:
        if(cmd->isExecuting()) {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::TextAlignmentRole:
        if (col == 0 || col == 1)
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
    }

    return QVariant();
}

QVariant CommandListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("État");
        case 1:
            return QString("Heure d'exécution");
        case 2:
            return QString("Commande");
        }
    }
    return QVariant();
}

bool CommandListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        Command *cmd = getCommand(index);
        if(cmd == nullptr)
            return false;

        if(index.column() == 2) {
            return cmd->setCommand(value.toString());
        }
        return false;
    }
    return false;
}

Qt::ItemFlags CommandListModel::flags(const QModelIndex &index) const
{
    const Command *cmd = getCommandConst(index);
    if(cmd == nullptr) return Qt::NoItemFlags;

    Qt::ItemFlags defFlags = QAbstractTableModel::flags(index);
    if(index.column() == 2) {
        return Qt::ItemIsEditable | defFlags;
    }
    else {
        return defFlags;
    }
}
