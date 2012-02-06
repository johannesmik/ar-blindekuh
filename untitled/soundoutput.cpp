#include "soundoutput.h"



soundoutput::soundoutput(QObject *parent):
    QObject(parent)
{
    playing = false;
    alutInit(NULL, NULL);

    ALfloat listenerPos[] = {0.0f, 0.0f, 0.0f};
    ALfloat listenerVel[] = {0.0f, 0.0f, 0.0f};
    ALfloat listenerOri[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};

    // Initialize ALUT
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);

    alGetError();

    // Generate buffers, or no sound will be produced
    alGenBuffers(1, &buffer);

    if(alGetError() != AL_NO_ERROR)
    {
        qDebug() << "- Error creating buffers !!";
    } else {
        qDebug() <<"Created buffers";
    }


    alGetError();
    alGenSources(1, &source);

    if(alGetError() != AL_NO_ERROR) {
        qDebug("- Error creating sources !!");
    } else {
        qDebug("Created sources");
    }

    //Setup default source parameters
    ALfloat sourcePos[]= {0.0f, 0.0f, 0.0f};
    ALfloat sourceVel[]= {0.0f, 0.0f, 0.0f};

    alSourcef(source,AL_PITCH,1.0f);
    alSourcef(source,AL_GAIN,1.0f);
    alSourcefv(source,AL_POSITION, sourcePos);
    alSourcefv(source,AL_VELOCITY, sourceVel);
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

void soundoutput::updateSourcePosition(ALfloat pos[])
{
    alSourcefv(source, AL_POSITION, pos);
}

void soundoutput::playSuccessSound()
{
    stop();
//    buffer = alutCreateBufferFromFile("..\..\win.wav");
//    qDebug() << "buffer" << (buffer) << alutGetErrorString (alutGetError ());
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, 523.251, 0.0, 0.20);
    alSourcei(source, AL_BUFFER, buffer);
    play();
    alutSleep(0.2);
    stop();
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, 659.255, 0.0, 0.20);
    alSourcei(source, AL_BUFFER, buffer);
    play();
    alutSleep(0.2);
    stop();
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, 783.991, 0.0, 0.20);
    alSourcei(source, AL_BUFFER, buffer);
    play();
    alutSleep(0.2);
    stop();
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, 1046.50, 0.0, 0.60);
    alSourcei(source, AL_BUFFER, buffer);
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
