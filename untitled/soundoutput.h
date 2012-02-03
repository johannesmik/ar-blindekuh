#ifndef SOUNDOUTPUT_H
#define SOUNDOUTPUT_H

#include <QObject>
#include <QTimer>
#include <vector>
#include <math.h>
#include <AL/alut.h>
#include <iostream>
#include "scenedescription.h"

class soundoutput : public QObject
{
    Q_OBJECT
public:
    soundoutput(scenedescription *s, QObject *parent = 0);
    void setPosition(std::vector<float> pos);
public slots:
    void playPause(int msecs);
    void stop();
    void play();

private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    QTimer timer;
    bool playing;
    ALuint	bufferNear;
    ALuint	source;
    scenedescription *scene;

};

#endif // SOUNDOUTPUT_H
