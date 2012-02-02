#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tracker = new MyMarkerTracker(this);

    // regularly query for marker positions
    frameQueryLoop.setInterval(50);
    connect(&frameQueryLoop, SIGNAL(timeout()), tracker, SLOT(queryForMarker()));
    connect(tracker, SIGNAL(frameUpdate(cv::Mat)), ui->cvwidget, SLOT(frameUpdate(cv::Mat)));

    frameQueryLoop.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
