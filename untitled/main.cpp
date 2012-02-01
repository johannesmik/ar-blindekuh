#include <QtGui/QApplication>
#include "mainwindow.h"
#include <GL/glut.h>
#include <AL/alut.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    alutInit (&argc, argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
