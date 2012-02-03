#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "scenedescription.h"
#include "soundoutput.h"
#include <vector>
#include <QStateMachine>
#include <QState>

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
public slots:
    void setPosition(std::vector<float> pos);
    void enterSearchMarker1();
    void startedGame();
    void defaultSound();
private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    scenedescription *scene;
    soundoutput *sound;
    std::vector<float> position;

    QStateMachine m;
    QState* state_initial;
    QState* search_marker1;
    QState* search_marker2;
    QState* search_marker3;
    QState* search_marker4;
    QState* state_finished;

    QTimer timer;
    
};

#endif // GAMELOGIC_H
