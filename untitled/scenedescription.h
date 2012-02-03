#ifndef SCENEDESCRIPTION_H
#define SCENEDESCRIPTION_H

#include <QObject>
#include <AL/alut.h>

class scenedescription : public QObject
{
    Q_OBJECT
public:
    explicit scenedescription(QObject *parent = 0);

    ALfloat listenerPos[3];
    ALfloat listenerVel[3];
    ALfloat	listenerOri[6];

    float marker0Pos[3];
    float marker1Pos[3];
    float marker2Pos[3];
    float marker3Pos[3];

    float player0Pos[3];
    float player0Dis;
    float player0Ang;

    ALfloat source0Pos[3];
    ALfloat source0Vel[3];

    float speaker0Pos[3];
    float speaker1Pos[3];
    float speaker2Pos[3];
    float speaker3Pos[3];

    
signals:
    
public slots:

private:
    void init();
    
};

#endif // SCENEDESCRIPTION_H
