#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tracker = new MyMarkerTracker(this);

    scene = new scenedescription(this);
    glWidget = new GLWidget(scene, this);
    ui->glLayout->addWidget(glWidget);
    sound = new soundoutput(scene, this);

    // regularly query for marker positions
    frameQueryLoop.setInterval(50);
    connect(&frameQueryLoop, SIGNAL(timeout()), tracker, SLOT(queryForMarker()));
    connect(tracker, SIGNAL(frameUpdate(cv::Mat)), ui->cvwidget, SLOT(frameUpdate(cv::Mat)));

    connect(tracker, SIGNAL(markerPositionUpdate(std::vector<QPair<std::vector<float>,int> >)), this, SLOT(markerUpdate(std::vector<QPair<std::vector<float>,int> >)));
    frameQueryLoop.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::markerUpdate(std::vector<QPair<std::vector<float>,int> > markers)
{
    for(int i = 0; i<markers.size(); i++){
        if(markers[i].second == 626){
            sound->setPosition(markers[i].first);
        }

    }
}


