#include "highscore.h"

highscore::highscore(QObject *parent) :
    QAbstractTableModel(parent)
{
}

highscore::~highscore()
{
    QSettings s;
}

int highscore::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return scores.size();
}

int highscore::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant highscore::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if(!index.isValid())
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    QPair<QString, qint64> item = scores[index.row()];
    if(index.column() == 0){
        return item.first;
    } else if(index.column() == 1) {
        return item.second;
    } else {
        return QVariant();
    }
}

void highscore::addHighscore(QString name, qint64 msecs)
{
    scores.append(QPair<QString, qint64>(name, msecs));
    emit layoutChanged();
}

void highscore::deleteHighscores()
{
    scores.clear();
    emit layoutChanged();
}

QVariant highscore::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Vertical)
        return QVariant();

    if(section == 0)
        return QVariant("Name");
    else if(section == 1)
        return QVariant("Score");
    return QVariant();

