#include "glwidget.h"
#include <GL/glut.h>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    //connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    //timer.start(100);
}

float speaker0Pos[]={ 2.0, 0.0, 2.0};
float speaker1Pos[]={ -2.0, 0.0, 2.0};
float speaker2Pos[]={ 2.0, 0.0, -2.0};
float speaker3Pos[]={ -2.0, 0.0, -2.0};

void GLWidget::initializeGL(){

    glClearColor(0.2 , 0.2 , 0.2 ,1);

}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

    glRotatef(30.0,11.0,1.0,0.0) ;

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
   // glRotatef(30.0,11.0,1.0,0.0) ;
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
