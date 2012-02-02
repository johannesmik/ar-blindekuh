#include "glwidget.h"
#include <GL/glut.h>
#include <iostream>
//#include "scenedescription.h"


int  activemarker = 0;
int  rate = 2000;
float frequency = 1000;
bool game_isactive = false;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{

//    qDebug("init GLWIDGET");

//    // Initialize ALUT
//    alListenerfv(AL_POSITION,listenerPos);
//    alListenerfv(AL_VELOCITY,listenerVel);
//    alListenerfv(AL_ORIENTATION,listenerOri);

//    alGetError();

//    // Generate buffers, or no sound will be produced
//    alGenBuffers(NUM_BUFFERS, buffer);

//    if(alGetError() != AL_NO_ERROR)
//    {
//        qDebug("- Error creating buffers !!");
//    }
//    else
//    {
//        qDebug("Created buffers");
//    }


//    alGetError();
//    alGenSources(NUM_SOURCES, source);

//    if(alGetError() != AL_NO_ERROR) {
//        qDebug("- Error creating sources !!");
//    } else {
//        qDebug("Created sources");
//    }

//    alSourcef(source[0],AL_PITCH,1.0f);
//    alSourcef(source[0],AL_GAIN,1.0f);
//    alSourcefv(source[0],AL_POSITION,source0Pos);
//    alSourcefv(source[0],AL_VELOCITY,source0Vel);
//    alSourcei(source[0],AL_BUFFER,buffer[0]);
//    alSourcei(source[0],AL_LOOPING,AL_FALSE);


//    // Set Timers
//    gametimer = new QTimer(this);
//    connect(gametimer, SIGNAL(timeout()), this, SLOT(game_mainloop()));
//    gametimer->start(30);

//    soundtimer = new QTimer(this);
//    connect(soundtimer, SIGNAL(timeout()), this, SLOT(sound_mainloop()));
//    soundtimer->start(1500);
}


void GLWidget::initializeGL(){

    glClearColor(0.2 , 0.2 , 0.2 ,1);

    glPushMatrix();
    glRotatef(30.0,11.0,1.0,0.0) ;
    glPopMatrix();

}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
//// Paint the Speakers
//    paintSpeaker(speaker0Pos, "Speaker 0");
//    paintSpeaker(speaker1Pos, "Speaker 1");
//    paintSpeaker(speaker2Pos, "Speaker 2");
//    paintSpeaker(speaker3Pos, "Speaker 3");

//// Paint the markers
//    paintMarker(marker0Pos, "Marker 0", activemarker == 0);
//    paintMarker(marker1Pos, "Marker 1", activemarker == 1);
//    paintMarker(marker2Pos, "Marker 2", activemarker == 2);
//    paintMarker(marker3Pos, "Marker 3", activemarker == 3);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,(GLsizei)width,(GLsizei)height) ;
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective(60.0,(GLfloat)width/(GLfloat)height,1.0,30.0) ;
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;
    glTranslatef(0.0,0.0,-6.6) ;
    glRotatef(30.0,11.0,1.0,0.0) ;
}

void GLWidget::print_text(GLfloat x, GLfloat y, GLfloat z, const char* text) {

    int len, i;
    len = (int) strlen(text);

    glPushMatrix();
    glRasterPos3f(x,y,z);
    glColor3f(1.0,1.0,1.0) ;
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text[i]);
    }
    glPopMatrix();
}


/*
    This method is responsible for the game logic
  */
void GLWidget::game_mainloop() {

   // qDebug("test mainloop aufgerufen");
    frequency -= 5;

    this->update();

}



void GLWidget::sound_mainloop() {

//    soundtimer->stop();
//    alSourceStop(source[0]);

//    calculate_frequency();
//    calculate_rate();

//    player0Ang -= 5.0;
//    player0Dis -= 0.5;

//    buffer[0] = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, frequency, 0.0, 0.5);
//    alSourcei(source[0],AL_BUFFER,buffer[0]);

//    qDebug("play sound");
//    alSourcePlay (source[0]);


//    // Restart the timer
//    soundtimer->start(rate);

}

void GLWidget::paintSpeaker(float speakerPos[], const char *name)
{
    glPushMatrix() ;
    glTranslatef(speakerPos[0],speakerPos[1],speakerPos[2]) ;
    glColor3f(1.0,0.0,0.0) ;
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(speakerPos[0], speakerPos[1], speakerPos[2], name);
}

void GLWidget::paintMarker(float markerPos[], const char *name, bool active)
{
    glPushMatrix() ;
    glTranslatef(markerPos[0],markerPos[1],markerPos[2]) ;
    if(active) {
        glColor3f(0.0,0.0,1.0) ;
    } else {
        glColor3f(0.0,1.0,0.0) ;
    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(markerPos[0], markerPos[1], markerPos[2], name);
}

