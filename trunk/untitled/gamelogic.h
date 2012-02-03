#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "scenedescription.h"
#include "soundoutput.h"
#include <vector>
#include <QStateMachine>
#include <QState>
#include <QDebug>
#include <QElapsedTimer>

class gamelogic : public QObject
{
    Q_OBJECT
public:
    explicit gamelogic(scenedescription* s, soundoutput *so, QObject *parent = 0);
    ~gamelogic();
    
signals:

    void found();
    void startGame();
    void newGame();
    void finishedGameIn(qint64 msecs);
public slots:
    void setPositions(std::vector<QPair<std::vector<float>,int> > pos);
    void enterSearchMarker1();
    void enterSearchMarker2();
    void enterSearchMarker3();
    void enterSearchMarker4();
    void finishedGame();
    void startedGame();
    void defaultSound();
private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    scenedescription *scene;
    soundoutput *sound;

    QStateMachine m;
    QState* state_initial;
    QState* search_marker1;
    QState* search_marker2;
    QState* search_marker3;
    QState* search_marker4;
    QState* state_finished;
    QState* allStates;
    QState* allSearchStates;

    QList<int> markerList;
    int currentMarker;

    QTimer defaultSoundTimer;
    QTimer nearRangeTimer;
    QElapsedTimer scoreTimer;
    
};

#endif // GAMELOGIC_H
