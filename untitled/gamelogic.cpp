#include "gamelogic.h"

#define MAX_FREQ 5000
#define MIN_FREQ 300
#define MAX_ANGLE 90
#define MIN_ANGLE 25

#define MAX_RATE 1000
#define MIN_RATE 40
#define MAX_DISTANCE 0.70f
#define MIN_DISTANCE 0.15f

gamelogic::gamelogic(scenedescription* s, QObject *parent) :
    QObject(parent)
{
    state_initial = new QState();
    search_marker1 = new QState();
    search_marker2 = new QState();
    search_marker3 = new QState();
    search_marker4 = new QState();
    state_finished = new QState();

    m.addState(state_initial);
    m.addState(search_marker1);
    m.addState(search_marker2);
    m.addState(search_marker3);
    m.addState(search_marker4);
    m.addState(state_finished);

    state_initial->addTransition(this, SIGNAL(startGame()), search_marker1);
    search_marker1->addTransition(this, SIGNAL(found()), search_marker2);
    search_marker2->addTransition(this, SIGNAL(found()), search_marker3);
    search_marker3->addTransition(this, SIGNAL(found()), search_marker4);
    search_marker4->addTransition(this, SIGNAL(found()), state_finished);

    connect(this, SIGNAL(startGame()), this, SLOT(enterSearchMarker1()));

    connect(search_marker1, SIGNAL(entered()), this, SLOT(enterSearchMarker1()));

    m.setInitialState(state_initial);
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



void gamelogic::setPosition(std::vector<float> pos)
{
    position = pos;

}

void gamelogic::newGame()
{
    m.start();
}

void gamelogic::enterSearchMarker1()
{
    qDebug("sdfsdfsdf");
}

