#include "soundoutput.h"
#include "scenedescription.h"


soundoutput::soundoutput(QObject *parent):
    QObject(parent)
{
    playing = false;

    alSourcef(source,AL_PITCH,1.0f);
    alSourcef(source,AL_GAIN,1.0f);
    alSourcefv(source,AL_POSITION, source0Pos);
    alSourcefv(source,AL_VELOCITY, source0Vel);
    alSourcei(source,AL_BUFFER, bufferNear);
    alSourcei(source,AL_LOOPING, AL_FALSE);
    timer.setInterval(1000);
    timer.setSingleShot(true);
    timer.start();
}

void soundoutput::setPosition(std::vector<float> pos)
{
    std::cout << pos[3] << " - " << pos[7] << " - " << pos[11] << std::endl;
    if(timer.isActive())
        return;

    std::cout << " -->>>  " << pos[11]*-400.0f << std::endl;
    stop();

    bufferNear = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, pos[11]*-400.0f, 0.0, 0.5);
    alSourcei(source, AL_BUFFER, bufferNear);
    play();
    timer.start();
}

void soundoutput::playPause(int msecs)
{
    if(playing)
        stop();
    else
        play();
}

void soundoutput::stop()
{
    playing = false;
    alSourceStop(source);
}

void soundoutput::play()
{
    playing = true;
    alSourcePlay(source);
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
