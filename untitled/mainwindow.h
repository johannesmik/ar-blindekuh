#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mymarkertracker.h"
#include <QTimer>
#include <QPair>
#include <vector>
#include "glwidget.h"
#include "soundoutput.h"
#include "gamelogic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
//    void markerUpdate(std::vector<QPair<std::vector<float>,int> > m);
    
private slots:

private:
    Ui::MainWindow *ui;
    MyMarkerTracker *tracker;
    QTimer frameQueryLoop;
    soundoutput *sound;
    GLWidget *glWidget;
    scenedescription *scene;
    gamelogic *game;
};

#endif // MAINWINDOW_H
