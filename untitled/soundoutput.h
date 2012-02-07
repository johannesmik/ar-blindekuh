#ifndef SOUNDOUTPUT_H
#define SOUNDOUTPUT_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <math.h>
#include <AL/alut.h>
#include <iostream>

class soundoutput : public QObject
{
    Q_OBJECT
public:
    soundoutput(QObject *parent = 0);
    void soundUpdate(float frequency);

public slots:
    void playPause(int msecs);
    void updateSourcePosition(ALfloat pos[]);
    void playMarkerFoundSound();
    void playSuccessSound();
    void stop();
    void play();

private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    bool playing;

    ALuint	buffer;
    ALuint	source;
    ALuint	buffer_notification;
    ALuint	source_notification;
};

#endif // SOUNDOUTPUT_H
