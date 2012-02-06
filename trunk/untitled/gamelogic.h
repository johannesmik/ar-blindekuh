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
#include "marker.h"

class gamelogic : public QObject
{
    Q_OBJECT
public:
    explicit gamelogic(soundoutput *so, QObject *parent = 0);
    ~gamelogic();
    
signals:

    void searchNext();
    void found();
    void foundAll();
    void startGame();
    void newGame();
    void finishedGameIn(qint64 msecs);
public slots:
    void handlePositionUpdate(std::vector<QPair<std::vector<float>,int> > pos);
    void enterSearchNextMarker();
    void foundMarker();
    void handleStartGame();
    void finishedGame();
    void handleNewGame();
    void defaultSound();
private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    soundoutput *sound;

    QStateMachine m;
    QState* state_initial;
    QState* searchNextMarker;
    QState* foundCurrentMarker;
    QState* state_finished;
    QState* allStates;
    QState* allSearchStates;

    QList<marker*> activeMarkers;
    QList<marker*> allMarkers;
    marker* currentMarker;

    QTimer defaultSoundTimer;
    QTimer nearRangeTimer;
    QElapsedTimer scoreTimer;
    
};

#endif // GAMELOGIC_H
