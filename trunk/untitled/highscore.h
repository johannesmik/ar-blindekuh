#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QObject>
#include <QSettings>
#include <QAbstractTableModel>
#include <QDebug>

class highscore : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit highscore(QObject *parent = 0);
    ~highscore();
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
//    virtual bool insertRows(int row, int count, const QModelIndex &parent);

signals:
    
public slots:
    void addHighscore(QString name, qint64 msecs);
    void deleteHighscores();
private:
    QList<QPair<QString, qint64> > scores;
    
};

#endif // HIGHSCORE_H
