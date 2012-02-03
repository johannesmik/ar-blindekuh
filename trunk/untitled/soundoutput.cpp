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
    alGenBuffers(1, &buffer);

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
    alSourcei(source,AL_BUFFER, buffer);
    alSourcei(source,AL_LOOPING, AL_FALSE);


}

void soundoutput::soundUpdate(float frequency)
{
    stop();
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, frequency, 0.0, 0.20);
    alSourcei(source, AL_BUFFER, buffer);
    play();
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
