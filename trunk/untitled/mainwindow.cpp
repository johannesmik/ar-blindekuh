#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    score = new highscore(this);
    QSortFilterProxyModel *filter = new QSortFilterProxyModel(this);
    filter->setSourceModel(score);
    ui->tableView->setModel(filter);

    tracker = new MyMarkerTracker(this);
    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), tracker, SLOT(setThreshold(int)));
    ui->verticalSlider->setValue(127);

    scene = new scenedescription(this);
    glWidget = new GLWidget(scene, this);
    ui->glLayout->addWidget(glWidget);
    sound = new soundoutput(this);

    game = new gamelogic(sound, this);

    // regularly query for marker positions
    frameQueryLoop.setInterval(50);
    connect(&frameQueryLoop, SIGNAL(timeout()), tracker, SLOT(queryForMarker()));

    connect(tracker, SIGNAL(frameUpdate(cv::Mat)), ui->cvwidget, SLOT(frameUpdate(cv::Mat)));
    connect(tracker, SIGNAL(frameUpdateBw(cv::Mat)), ui->cvwidget, SLOT(frameUpdateBw(cv::Mat)));

    connect(tracker, SIGNAL(markerPositionUpdate(std::vector<QPair<std::vector<float>,int> >)),
            game, SLOT(handlePositionUpdate(std::vector<QPair<std::vector<float>,int> >)));

    connect(ui->startGame, SIGNAL(clicked()), game, SIGNAL(startGame()));
    connect(ui->restartGame, SIGNAL(clicked()), game, SIGNAL(newGame()));
    connect(game, SIGNAL(finishedGameIn(qint64)), this, SLOT(handleFinishedGame(qint64)));
    connect(ui->deleteHighscoreButton, SIGNAL(clicked()), score, SLOT(deleteHighscores()));

    frameQueryLoop.start();
}

void MainWindow::handleFinishedGame(qint64 msecs)
{
    bool ok;
    QString q = QString("Du hast ").append(QString::number(msecs/1000)).append(" Sekunden gebraucht. Wie ist dein Name?");
    QString name = QInputDialog::getText(this, "Highscore eintragen",q, QLineEdit::Normal,"sfsdfdsf", &ok);
    if(ok && !name.isEmpty()){
        score->addHighscore(name, msecs);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
