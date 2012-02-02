#ifndef SOUNDOUTPUT_H
#define SOUNDOUTPUT_H

#include <QObject>
#include <QTimer>
#include <vector>
#include <math.h>

class soundoutput : public QObject
{
    Q_OBJECT
public:
    soundoutput(QObject *parent = 0);
    void setPosition(std::vector<float> pos);
public slots:
    void playPause(int msecs);
    void stop();

private:
    float calculateFrequency(float angle);
    float calculateRate(float distance);
    QTimer timer;
    bool playingState;
};

#endif // SOUNDOUTPUT_H
