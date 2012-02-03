#include "soundoutput.h"



soundoutput::soundoutput(scenedescription *s, QObject *parent):
    QObject(parent)
{
    scene = s;
    playing = false;

    // Initialize ALUT
    alListenerfv(AL_POSITION, scene->listenerPos);
    alListenerfv(AL_VELOCITY, scene->listenerVel);
    alListenerfv(AL_ORIENTATION, scene->listenerOri);

    alGetError();

    // Generate buffers, or no sound will be produced
    alGenBuffers(1, &bufferNear);

    if(alGetError() != AL_NO_ERROR)
    {
        qDebug("- Error creating buffers !!");
    }
    else
    {
        qDebug("Created buffers");
    }


    alGetError();
    alGenSources(1, &source);

    if(alGetError() != AL_NO_ERROR) {
        qDebug("- Error creating sources !!");
    } else {
        qDebug("Created sources");
    }

    alSourcef(source,AL_PITCH,1.0f);
    alSourcef(source,AL_GAIN,1.0f);
    alSourcefv(source,AL_POSITION, scene->source0Pos);
    alSourcefv(source,AL_VELOCITY, scene->source0Vel);
    alSourcei(source,AL_BUFFER, bufferNear);
    alSourcei(source,AL_LOOPING, AL_FALSE);

    timer.setInterval(1000);
    timer.setSingleShot(true);
    timer.start();
    maxdist = 0;
    mindist = 100000;
}

#define PI 3.14159265f
#define MAX_FREQ 5000
#define MIN_FREQ 300
#define MAX_ANGLE 90
#define MIN_ANGLE 25

#define MAX_RATE 1000
#define MIN_RATE 40
#define MAX_DISTANCE 0.70f
#define MIN_DISTANCE 0.15f



void soundoutput::setPosition(std::vector<float> pos)
{
    float distance = sqrt(pos[3]*pos[3] + pos[7]*pos[7] + pos[11] * pos[11]);
    if (distance > maxdist)
        maxdist = distance;

    if (distance < mindist)
        mindist = distance;

    float crossproduct = -pos[10]/sqrt(pos[2]*pos[2] + pos[6]*pos[6] + pos[10]*pos[10]);
    float angle = acosf(crossproduct);
    float angleDegree =180/(PI)*angle;

    std::cout << pos[3] << " - " << pos[7] << " - " << pos[11] << std::endl;
    std::cout << " -->>>  distance: " << distance <<  "(max: "<< maxdist <<", min: "<< mindist <<")" << std::endl;
    std::cout << " -->>>  crossproduct: " << crossproduct << std::endl;
    std::cout << " -->>>  arccos: " << angle << ", degrees: " << angleDegree <<std::endl;

    if(timer.isActive())
        return;

    stop();

    bufferNear = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, calculateFrequency(angleDegree), 0.0, 0.20);
    alSourcei(source, AL_BUFFER, bufferNear);
    play();
    timer.start(calculateRate(distance));
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

    if(angle >= MAX_ANGLE) {
        angle = MAX_ANGLE;
    }

    float halbton = floor ((MAX_ANGLE - angle) / 4);

    // C = 261.63 Hz, Cis 277.18, D 293.88, Es 311.13,
    // E 329.63, F 349.23, Fis 370, G 392
    // As 415.3, A 440, B 466.16, H 493.88
    float frequency = MIN_FREQ * pow(2.0f, (halbton/6));
    qDebug("frequenz ist %f", frequency);

    return frequency;
}

float soundoutput::calculateRate(float distance) {


    if(distance >= MAX_DISTANCE) {
        distance = MAX_DISTANCE;
    }

    float rate = MIN_RATE + (MAX_RATE - MIN_RATE)*distance/MAX_DISTANCE;

    qDebug("rate ist %f", rate);

    return rate;
}
