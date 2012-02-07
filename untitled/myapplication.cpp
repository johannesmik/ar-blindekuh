#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

bool MyApplication::notify(QObject *receiver, QEvent *event)
{
    try {
      return QApplication::notify(receiver, event);
    } catch(std::exception& e) {
      if(!QString(e.what()).contains("matrix.cpp, line 1219")){
          qCritical() << "Exception thrown:" << e.what();
      }
    }
    return false;
}
