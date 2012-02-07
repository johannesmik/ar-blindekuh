#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QDebug>

class MyApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MyApplication(int & argc, char ** argv);
    virtual bool notify(QObject * receiver, QEvent * event);
signals:
    
public slots:
    
};

#endif // MYAPPLICATION_H
