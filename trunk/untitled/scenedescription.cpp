#include "scenedescription.h"

scenedescription::scenedescription(QObject *parent) :
    QObject(parent)
{
}

void scenedescription::init()
{
    listenerPos[0]=0.0;
    listenerPos[1]=0.0;
    listenerPos[2]=0.0;

    listenerVel[0]=0.0;
    listenerVel[1]=0.0;
    listenerVel[2]=0.0;

    listenerOri[0]=0.0;
    listenerOri[1]=0.0;
    listenerOri[2]=-1.0;
    listenerOri[3]=0.0;
    listenerOri[4]=1.0;
    listenerOri[5]=0.0;

    marker0Pos[0]=2.0;
    marker0Pos[1]=0.0;
    marker0Pos[2]=0.0;

    marker1Pos[0]=0.0;
    marker1Pos[1]=0.0;
    marker1Pos[2]=2.0;

    marker2Pos[0]=-2.0;
    marker2Pos[1]=0.0;
    marker2Pos[2]=0.0;

    marker3Pos[0]=0.0;
    marker3Pos[1]=0.0;
    marker3Pos[2]=-2.0;

    player0Pos[0]=0.0;
    player0Pos[1]=0.0;
    player0Pos[2]=0.0;

    player0Dis = 6.0;
    player0Ang = 90.0;

    source0Pos[0]=0.0;
    source0Pos[1]=0.0;
    source0Pos[2]=-2.0;

    source0Vel[0]=0.0;
    source0Vel[1]=0.0;
    source0Vel[2]=0.0;

    speaker0Pos[0]=+2.0;
    speaker0Pos[1]=0.0;
    speaker0Pos[2]=+2.0;

    speaker1Pos[0]=-2.0;
    speaker1Pos[1]=0.0;
    speaker1Pos[2]=+2.0;

    speaker2Pos[0]=-2.0;
    speaker2Pos[1]=0.0;
    speaker2Pos[2]=-2.0;

    speaker3Pos[0]=2.0;
    speaker3Pos[1]=0.0;
    speaker3Pos[2]=2.0;
}



