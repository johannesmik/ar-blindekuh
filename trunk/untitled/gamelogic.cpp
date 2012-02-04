#include "gamelogic.h"

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

gamelogic::gamelogic(scenedescription* s,  soundoutput *so, QObject *parent) :
    QObject(parent)
{
    scene = s;
    sound = so;

    markerList << 626 << 1680 << 90 << 7236;

    allStates = new QState();
    state_initial = new QState(allStates);
    search_marker1 = new QState(allStates);
    search_marker2 = new QState(allStates);
    search_marker3 = new QState(allStates);
    search_marker4 = new QState(allStates);
    state_finished = new QState(allStates);

    m.addState(state_initial);
    m.addState(search_marker1);
    m.addState(search_marker2);
    m.addState(search_marker3);
    m.addState(search_marker4);
    m.addState(state_finished);

    m.setInitialState(state_initial);

    state_initial->addTransition(this, SIGNAL(startGame()), search_marker1);
    search_marker1->addTransition(this, SIGNAL(found()), search_marker2);
    search_marker2->addTransition(this, SIGNAL(found()), search_marker3);
    search_marker3->addTransition(this, SIGNAL(found()), search_marker4);
    search_marker4->addTransition(this, SIGNAL(found()), state_finished);

    search_marker1->addTransition(this, SIGNAL(newGame()), state_initial);
    search_marker2->addTransition(this, SIGNAL(newGame()), state_initial);
    search_marker3->addTransition(this, SIGNAL(newGame()), state_initial);
    search_marker4->addTransition(this, SIGNAL(newGame()), state_initial);
    state_finished->addTransition(this, SIGNAL(newGame()), state_initial);


    connect(search_marker1, SIGNAL(entered()), this, SLOT(enterSearchMarker1()));
    connect(search_marker2, SIGNAL(entered()), this, SLOT(enterSearchMarker2()));
    connect(search_marker3, SIGNAL(entered()), this, SLOT(enterSearchMarker3()));
    connect(search_marker4, SIGNAL(entered()), this, SLOT(enterSearchMarker4()));
    connect(state_initial, SIGNAL(entered()), this, SLOT(startedGame()));
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
    delete search_marker1;
    delete search_marker2;
    delete search_marker3;
    delete search_marker4;
    delete state_finished;
}



void gamelogic::setPositions(std::vector<QPair<std::vector<float>,int> > markers)
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
        if(markers[i].second == currentMarker){
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

void gamelogic::enterSearchMarker1()
{
    scoreTimer.start();
    defaultSoundTimer.start(0);
    currentMarker = markerList[0];
    qDebug("marker1!!");
}

void gamelogic::enterSearchMarker2()
{
    defaultSoundTimer.start(0);
    currentMarker = markerList[1];
    qDebug("marker2!!");
}

void gamelogic::enterSearchMarker3()
{
    defaultSoundTimer.start(0);
    currentMarker = markerList[2];
    qDebug("marker3!!");
}

void gamelogic::enterSearchMarker4()
{
    defaultSoundTimer.start(0);
    currentMarker = markerList[3];
    qDebug("marker4!!");
}

void gamelogic::finishedGame()
{
    qint64 elapsedTime = scoreTimer.elapsed();
    qDebug() << "elapsed time" << elapsedTime;
    defaultSoundTimer.stop();
    emit(newGame());
    emit(finishedGameIn(elapsedTime));
}

void gamelogic::startedGame()
{
    qDebug("New Game starts!!");
    defaultSoundTimer.stop();
}

void gamelogic::defaultSound()
{
    qDebug("Play default sound");
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

