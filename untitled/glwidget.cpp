#include "glwidget.h"
#include <GL/glut.h>
#include <iostream>


int  activemarker = 0;
int  rate = 2000;
float frequency = 1000;
bool game_isactive = false;
float color_white[] = {1.0,1.0,1.0,1.0};
float shininess_normal[]  = {55.0};
float light1_position[] = {1,5,1,0};
float light2_position[] = {1,-3,1,0};
float color_blue[] = {0,0,1,0};
float color_red[] = {1,0,0,0};
float color_green[] = {0,1,0,0};

GLWidget::GLWidget(scenedescription *s, QWidget *parent) :
    QGLWidget(parent)
{
    scene = s;

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
    position.resize(16);
}


void GLWidget::initializeGL(){

    glClearColor(0.2 , 0.2 , 0.2 ,1);

    glPushMatrix();
    glRotatef(30.0,11.0,1.0,0.0) ;
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_SPECULAR, color_white);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color_blue);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_normal);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color_white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color_white);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 30);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);

}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
// Paint the Speakers
    paintSpeaker(scene->speaker0Pos, "Speaker 0");
    paintSpeaker(scene->speaker1Pos, "Speaker 1");
    paintSpeaker(scene->speaker2Pos, "Speaker 2");
    paintSpeaker(scene->speaker3Pos, "Speaker 3");

// Paint the markers
    paintMarker(scene->marker0Pos, "Marker 0", activemarker == 0);
    paintMarker(scene->marker1Pos, "Marker 1", activemarker == 1);
    paintMarker(scene->marker2Pos, "Marker 2", activemarker == 2);
    paintMarker(scene->marker3Pos, "Marker 3", activemarker == 3);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glColor3f(0.0,1.0,0.0);
    glMultMatrixf(&position[0]);
    glutSolidTeapot(0.22);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
    glPopMatrix() ;


    glPushMatrix();
    glLineWidth(2);
    glLoadIdentity();
    glColor3f(1.0,1.0,1.0) ;

    glPopMatrix();

//    glutSolidTeapot(0.22);
//    glTranslated(0,0,-3);
//   // ORIGINAL LINUX CODE
//   // glMultTransposeMatrixf(markers[626].data());
//   // Windows Code: ?
//    glMultMatrixf(&markers[626][0]);
//    glRotatef(180, 1, 0,0);
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

void GLWidget::setPosition(std::vector<float> pos)
{
    position[0] = pos[0];
    position[1] = pos[4];
    position[2] = pos[8];
    position[3] = pos[12];

    position[4] = pos[1];
    position[5] = pos[5];
    position[6] = pos[9];
    position[7] = pos[13];

    position[8] = pos[2];
    position[9] = pos[6];
    position[10] = pos[10];
    position[11] = pos[14];

    position[12] = pos[3];
    position[13] = pos[7];
    position[14] = pos[11];
    position[15] = pos[15];

    update();
}

void GLWidget::paintSpeaker(float speakerPos[], const char *name)
{
    glPushMatrix() ;
    glTranslatef(speakerPos[0],speakerPos[1],speakerPos[2]) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color_red);
    glutWireCube(0.5) ;
    glPopMatrix();
    print_text(speakerPos[0], speakerPos[1], speakerPos[2], name);
}

void GLWidget::paintMarker(float markerPos[], const char *name, bool active)
{
    glPushMatrix() ;
    glTranslatef(markerPos[0],markerPos[1],markerPos[2]) ;
    if(active) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color_blue);
    } else {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color_green);

    }
    glutWireCube(0.5) ;
    glPopMatrix() ;
    print_text(markerPos[0], markerPos[1], markerPos[2], name);
}

