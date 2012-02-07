#include "highscore.h"
#include <QFile>

#define HIGHSCORES_FILE "highscores.dat"

highscore::highscore(QObject *parent) :
    QAbstractTableModel(parent)
{
    QFile f(HIGHSCORES_FILE);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    in >> scores; // extract from file
    f.close();
}

highscore::~highscore()
{
    QFile f(HIGHSCORES_FILE);
    f.open(QIODevice::WriteOnly);
    QDataStream out(&f);   // we will serialize the data into the file
    out << scores;
    f.close();
}

int highscore::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return scores.size();
}

int highscore::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
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
        double secs = item.second;
        secs /= 1000;
        return QString("%1").arg(QString::number(secs, 'f', 2));
    } else {
        return QVariant();
    }
}

void highscore::addHighscore(QString name, qint64 msecs)
{
    scores.append(QPair<QString, qint64>(name, msecs));
    emit dataChanged(index(0,0), index(scores.size()-1, 1));
    emit layoutChanged();
}

void highscore::deleteHighscores()
{
    scores.clear();
    emit dataChanged(index(0,0), index(scores.size()-1, 1));
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
}
