#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "scenedescription.h"
#include <vector>
#include <QStateMachine>
#include <QState>

class gamelogic : public QObject
{
    Q_OBJECT
public:
    explicit gamelogic(scenedescription* s, QObject *parent = 0);
    ~gamelogic();
    
signals:

    void found();
    void startGame();
public slots:
    void setPosition(std::vector<float> pos);
    void newGame();
    void enterSearchMarker1();
private:
    scenedescription *scene;
    std::vector<float> position;

    QStateMachine m;
    QState* state_initial;
    QState* search_marker1;
    QState* search_marker2;
    QState* search_marker3;
    QState* search_marker4;
    QState* state_finished;
    
};

#endif // GAMELOGIC_H
