#include "scenedescription.h"

scenedescription::scenedescription(QObject *parent) :
    QObject(parent)
{
    init();
}

void scenedescription::init()
{
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
    speaker3Pos[2]=-2.0;
}



