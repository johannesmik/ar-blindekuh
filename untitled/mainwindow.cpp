#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scoreLabel->setText("");
    tracker = new MyMarkerTracker(this);

    scene = new scenedescription(this);
    glWidget = new GLWidget(scene, this);
    ui->glLayout->addWidget(glWidget);
    sound = new soundoutput(scene, this);

    game = new gamelogic(scene, sound, this);

    // regularly query for marker positions
    frameQueryLoop.setInterval(50);
    connect(&frameQueryLoop, SIGNAL(timeout()), tracker, SLOT(queryForMarker()));

    connect(tracker, SIGNAL(frameUpdate(cv::Mat)), ui->cvwidget, SLOT(frameUpdate(cv::Mat)));
    connect(tracker, SIGNAL(frameUpdateBw(cv::Mat)), ui->cvwidget, SLOT(frameUpdateBw(cv::Mat)));

    connect(tracker, SIGNAL(markerPositionUpdate(std::vector<QPair<std::vector<float>,int> >)),
            game, SLOT(setPositions(std::vector<QPair<std::vector<float>,int> >)));

    connect(ui->startGame, SIGNAL(clicked()), game, SIGNAL(startGame()));
    connect(ui->restartGame, SIGNAL(clicked()), game, SIGNAL(newGame()));
    connect(game, SIGNAL(finishedGameIn(qint64)), this, SLOT(updateLabel(qint64)));

    frameQueryLoop.start();
}

void MainWindow::updateLabel(qint64 msecs)
{
    ui->scoreLabel->setText(QString("Oh yeah, du hast es in ").append(QString::number(msecs)).append(" Sekunden geschafft. "));
}

MainWindow::~MainWindow()
{
    delete ui;
}
