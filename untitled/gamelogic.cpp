#include "gamelogic.h"
#include <QDateTime>

#define PI 3.14159265

#define DEFAULT_FREQ 300
#define DEFAULT_RATE 800

#define MAX_FREQ 5000
#define MIN_FREQ 400
#define MAX_ANGLE 90
#define MIN_ANGLE 25


#define MAX_RATE 1000
#define MIN_RATE 40
#define MAX_DISTANCE 0.70f
#define MIN_DISTANCE 0.15f

gamelogic::gamelogic(soundoutput *so, QObject *parent) :
    QObject(parent)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    sound = so;

    marker *m1 = new marker(626,  2, 0, 0, this);
    marker *m2 = new marker(1680, 0, 0, 2, this);
    marker *m3 = new marker(90,  -2, 0, 0, this);
    marker *m4 = new marker(7236, 0, 0,-2, this);

    allMarkers << m1 << m2 << m3 << m4;

    allStates = new QState();
    state_initial = new QState(allStates);
    searchNextMarker = new QState(allStates);
    foundCurrentMarker = new QState(allStates);
    state_finished = new QState(allStates);
    allStates->setInitialState(state_initial);

    m.addState(allStates);

    m.setInitialState(allStates);

    state_initial->addTransition(this, SIGNAL(startGame()), searchNextMarker);
    searchNextMarker->addTransition(this, SIGNAL(found()), foundCurrentMarker);
    foundCurrentMarker->addTransition(this, SIGNAL(foundAll()), state_finished);
    foundCurrentMarker->addTransition(this, SIGNAL(searchNext()), searchNextMarker);
    allStates->addTransition(this, SIGNAL(newGame()), state_initial);


    connect(searchNextMarker, SIGNAL(entered()), this, SLOT(enterSearchNextMarker()));
    connect(foundCurrentMarker, SIGNAL(entered()), this, SLOT(foundMarker()));
    connect(state_initial, SIGNAL(entered()), this, SLOT(handleNewGame()));
    connect(state_initial, SIGNAL(exited()), this, SLOT(handleStartGame()));
    connect(state_finished, SIGNAL(entered()), this, SLOT(finishedGame()));

    m.start();


    defaultSoundTimer.setSingleShot(true);
    connect(&defaultSoundTimer, SIGNAL(timeout()), this, SLOT(defaultSound()));

    nearRangeTimer.setInterval(0);
    nearRangeTimer.setSingleShot(true);
    nearRangeTimer.start();
}

gamelogic::~gamelogic()
{
    delete state_initial;

    delete state_finished;
    delete searchNextMarker;
    delete foundCurrentMarker;
}



void gamelogic::handlePositionUpdate(std::vector<QPair<std::vector<float>,int> > markers)
{
    QSet<QAbstractState*> set = m.configuration();
    if(set.contains(state_initial) || set.contains(state_finished)){
        return;
    }
    if(nearRangeTimer.isActive()){
        //currently paused
        return;
    }

    std::vector<float> pos;
    int i;
    for(i = 0; i<markers.size(); i++){
        if(markers[i].second == currentMarker->getMarkerID()){
            pos = markers[i].first;
            break;
        }
    }
    if(i == markers.size()) // did not find the current marker, so just do nothing
        return;

    float distance = sqrt(pos[3]*pos[3] + pos[7]*pos[7] + pos[11] * pos[11]);

    float crossproduct = -pos[10]/sqrt(pos[2]*pos[2] + pos[6]*pos[6] + pos[10]*pos[10]);
    float angle = acosf(crossproduct);
    float angleDegree =180/(PI)*angle;


//    std::cout << pos[3] << " - " << pos[7] << " - " << pos[11] << std::endl;
    std::cout << " -->>>  distance: " << distance << std::endl;
//    std::cout << " -->>>  crossproduct: " << crossproduct << std::endl;
//    std::cout << " -->>>  arccos: " << angle <<std::endl;
    std::cout << " -->>>  degrees: " << angleDegree <<std::endl;

    if(distance < MIN_DISTANCE && angleDegree < MIN_ANGLE){
        qDebug() << "found Marker" << currentMarker;
        emit(found());
        return;
    }

    sound->soundUpdate(calculateFrequency(angleDegree));
    nearRangeTimer.start(calculateRate(distance));
    defaultSoundTimer.start(3000);
}

void gamelogic::enterSearchNextMarker()
{
    currentMarker = activeMarkers.takeFirst();
    sound->updateSourcePosition(currentMarker->getMarkerPos());
    defaultSoundTimer.start(0);
    float *pos=currentMarker->getMarkerPos();
    qDebug() << "search for marker" << currentMarker->getMarkerID() << "@" << pos[0] <<","<< pos[1] <<","<< pos[2];
}

void gamelogic::finishedGame()
{
    qint64 elapsedTime = scoreTimer.elapsed();
    qDebug() << "elapsed time" << elapsedTime;
    defaultSoundTimer.stop();
    emit(newGame());
    emit(finishedGameIn(elapsedTime));
}

void gamelogic::handleNewGame()
{
    qDebug("New Game starts!!");
    defaultSoundTimer.stop();
}

void gamelogic::defaultSound()
{
//    qDebug("Play default sound");
    sound->soundUpdate(DEFAULT_FREQ);
    defaultSoundTimer.start(DEFAULT_RATE);
}

float gamelogic::calculateFrequency(float angle) {

    if(angle >= MAX_ANGLE) {
        angle = MAX_ANGLE;
    }

    float halbton = floor ((MAX_ANGLE - angle) / 4);

    // C = 261.63 Hz, Cis 277.18, D 293.88, Es 311.13,
    // E 329.63, F 349.23, Fis 370, G 392
    // As 415.3, A 440, B 466.16, H 493.88
    float frequency = MIN_FREQ * pow(2.0f, (halbton/6));
    //qDebug("frequenz ist %f", frequency);

    return frequency;
}

float gamelogic::calculateRate(float distance) {


    if(distance >= MAX_DISTANCE) {
        distance = MAX_DISTANCE;
    }

    float rate = MIN_RATE + (MAX_RATE - MIN_RATE)*distance/MAX_DISTANCE;

    //qDebug("rate ist %f", rate);

    return rate;
}

void gamelogic::foundMarker()
{
    if(activeMarkers.isEmpty()){
        emit foundAll();
    } else {
        emit searchNext();
    }
}

void gamelogic::handleStartGame()
{
    activeMarkers.clear();
    QList<marker*> markers = allMarkers;
    while(!markers.isEmpty()){
        int idx = qrand()%markers.size();
        activeMarkers.append(markers.takeAt(idx));
    }
    scoreTimer.start();
}

