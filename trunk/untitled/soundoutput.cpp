#include "soundoutput.h"
#include <AL/alut.h>

//ALfloat listenerPos[]={0.0,0.0,0.0};
//ALfloat listenerVel[]={0.0,0.0,0.0};
//ALfloat	listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};

//float marker0Pos[]={ 2.0, 0.0, 0.0};
//float marker1Pos[]={ 0.0, 0.0, 2.0};
//float marker2Pos[]={ -2.0, 0.0, 0.0};
//float marker3Pos[]={ 0.0, 0.0, -2.0};

//float player0Pos[]={0.0, 0.0, 0.0};
//float player0Dis = 6.0;
//float player0Ang = 90.0;

//ALfloat source0Pos[]={ 0.0, 0.0, -2.0};
//ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

//float speaker0Pos[]={ 2.0, 0.0, 2.0};
//float speaker1Pos[]={ -2.0, 0.0, 2.0};
//float speaker2Pos[]={ -2.0, 0.0, -2.0};
//float speaker3Pos[]={ 2.0, 0.0, -2.0};

soundoutput::soundoutput(QObject *parent):
    QObject(parent)
{
    playingState = false;
}

void soundoutput::setPosition(std::vector<float> pos)
{

}

void soundoutput::playPause(int msecs)
{
}

void soundoutput::stop()
{
    playingState = false;
}



float soundoutput::calculateFrequency(float angle) {

    if(angle >= 50.0) {
        angle = 50.0;
    }
    if(angle <= 0.0) {
        angle = 0.0;
    }

    float halbton = floor (angle / 5);

    // C = 261.63 Hz, Cis 277.18, D 293.88, Es 311.13,
    // E 329.63, F 349.23, Fis 370, G 392
    // As 415.3, A 440, B 466.16, H 493.88
    float frequency = 261.63 * pow(2.0f, (halbton/12));
    qDebug("frequenz ist %f", frequency);

    return frequency;
}

float soundoutput::calculateRate(float distance) {


    if(distance >= 5.0) {
        distance = 5.0;
    }
    if(distance <= 0.0) {
        distance = 0.01;
    }

    float rate = (int) ceil (300 + 600 * distance);
    if(rate <= 300) {
        rate = 300;
    }
    qDebug("rate ist %i", rate);

    return rate;
}
