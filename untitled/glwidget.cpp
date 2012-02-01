#include "glwidget.h"
#include <GL/glut.h>
#include <iostream>

ALfloat listenerPos[]={0.0,0.0,0.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat	listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};

float marker0Pos[]={ 2.0, 0.0, 0.0};
float marker1Pos[]={ 0.0, 0.0, 2.0};
float marker2Pos[]={ -2.0, 0.0, 0.0};
float marker3Pos[]={ 0.0, 0.0, -2.0};

float player0Pos[]={0.0, 0.0, 0.0};
float player0Dis = 6.0;
float player0Ang = 90.0;

ALfloat source0Pos[]={ 0.0, 0.0, -2.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

float speaker0Pos[]={ 2.0, 0.0, 2.0};
float speaker1Pos[]={ -2.0, 0.0, 2.0};
float speaker2Pos[]={ -2.0, 0.0, -2.0};
float speaker3Pos[]={ 2.0, 0.0, -2.0};


int  activemarker = 0;
int  rate = 2000;
float frequency = 1000;
bool game_isactive = false;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{

    qDebug("init GLWIDGET");

    // Initialize ALUT
    alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

    alGetError();

    // Generate buffers, or no sound will be produced
    alGenBuffers(NUM_BUFFERS, buffer);

    if(alGetError() != AL_NO_ERROR)
    {
        qDebug("- Error creating buffers !!");
    }
    else
    {
        qDebug("Created buffers");
    }


    alGetError();
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR) {
        qDebug("- Error creating sources !!");
    } else {
        qDebug("Created sources");
    }

    alSourcef(source[0],AL_PITCH,1.0f);
    alSourcef(source[0],AL_GAIN,1.0f);
    alSourcefv(source[0],AL_POSITION,source0Pos);
    alSourcefv(source[0],AL_VELOCITY,source0Vel);
    alSourcei(source[0],AL_BUFFER,buffer[0]);
    alSourcei(source[0],AL_LOOPING,AL_FALSE);


    // Set Timers
    gametimer = new QTimer(this);
    connect(gametimer, SIGNAL(timeout()), this, SLOT(game_mainloop()));
    gametimer->start(30);

    soundtimer = new QTimer(this);
    connect(soundtimer, SIGNAL(timeout()), this, SLOT(sound_mainloop()));
    soundtimer->start(1500);
}


void GLWidget::initializeGL(){

    glClearColor(0.2 , 0.2 , 0.2 ,1);

    glPushMatrix();
    glRotatef(30.0,11.0,1.0,0.0) ;
    glPopMatrix();

}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;



// Paint the Speakers

    glPushMatrix() ;
    glTranslatef(speaker0Pos[0],speaker0Pos[1],speaker0Pos[2]) ;
    glColor3f(1.0,0.0,0.0) ;
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(speaker0Pos[0], speaker0Pos[1], speaker0Pos[2], "Speaker 0");

    glPushMatrix() ;
    glTranslatef(speaker1Pos[0],speaker1Pos[1],speaker1Pos[2]) ;
    glColor3f(1.0,0.0,0.0) ;
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(speaker1Pos[0], speaker1Pos[1], speaker1Pos[2], "Speaker 1");


    glPushMatrix() ;
    glTranslatef(speaker2Pos[0],speaker2Pos[1],speaker2Pos[2]) ;
    glColor3f(1.0,0.6,0.0) ;
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(speaker2Pos[0], speaker2Pos[1], speaker2Pos[2], "Speaker 2");

    glPushMatrix() ;
    glTranslatef(speaker3Pos[0],speaker3Pos[1],speaker3Pos[2]) ;
    glColor3f(1.0,0.6,0.0) ;
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(speaker3Pos[0], speaker3Pos[1], speaker3Pos[2], "Speaker 3");

// Paint the markers
    glPushMatrix() ;
    glTranslatef(marker0Pos[0],marker0Pos[1],marker0Pos[2]) ;
    if(activemarker == 0) {
        glColor3f(0.0,0.0,1.0) ;
    } else {
        glColor3f(0.0,1.0,0.0) ;
    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(marker0Pos[0], marker0Pos[1], marker0Pos[2], "marker 0");

    glPushMatrix() ;
    glTranslatef(marker1Pos[0],marker1Pos[1],marker1Pos[2]) ;
    if(activemarker == 1) {
        glColor3f(0.0,0.0,1.0) ;
    } else {
        glColor3f(0.0,1.0,0.0) ;
    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(marker1Pos[0], marker1Pos[1], marker1Pos[2], "marker 1");


    glPushMatrix() ;
    glTranslatef(marker2Pos[0],marker2Pos[1],marker2Pos[2]) ;
    if(activemarker == 2) {
        glColor3f(0.0,0.0,1.0) ;
    } else {
        glColor3f(0.0,1.0,0.0) ;
    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(marker2Pos[0], marker2Pos[1], marker2Pos[2], "marker 2");

    glPushMatrix() ;
    glTranslatef(marker3Pos[0],marker3Pos[1],marker3Pos[2]) ;
    if(activemarker == 3) {
        glColor3f(0.0,0.0,1.0) ;
    } else {
        glColor3f(0.0,1.0,0.0) ;
    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(marker3Pos[0], marker3Pos[1], marker3Pos[2], "marker 3");

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

void GLWidget::print_text(GLfloat x, GLfloat y, GLfloat z, char* text) {

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

    soundtimer->stop();
    alSourceStop(source[0]);

    calculate_frequency();
    calculate_rate();

    player0Ang -= 5.0;
    player0Dis -= 0.5;

    buffer[0] = alutCreateBufferWaveform(ALUT_WAVEFORM_SAWTOOTH, frequency, 0.0, 0.5);
    alSourcei(source[0],AL_BUFFER,buffer[0]);

    qDebug("play sound");
    alSourcePlay (source[0]);


    // Restart the timer
    soundtimer->start(rate);

}
void GLWidget::calculate_frequency() {

    float angle = player0Ang;

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
    frequency = 261.63 * pow(2.0, (halbton/12));
    qDebug("frequenz ist %f", frequency);


}
void GLWidget::calculate_rate() {

    float dis = player0Dis;

    if(dis >= 5.0) {
        dis = 5.0;
    }
    if(dis <= 0.0) {
        dis = 0.01;
    }

    rate = (int) ceil (300 + 600 * dis);
    if(rate <= 300) {
        rate = 300;
    }
    qDebug("rate ist %i", rate);

}
