#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <AL/alut.h>

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#include <QGLWidget>
#include <QTimer>

#include <cmath>
#include "scenedescription.h"
#include <QDebug>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(scenedescription* s, QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

public slots:
    void game_mainloop();
    void sound_mainloop();

private:
    void paintSpeaker(float speakerPos[], const char *name);
    void paintMarker(float markerPos[], const char *name, bool active);

    QTimer *gametimer;
    QTimer *soundtimer;
    void print_text(GLfloat x, GLfloat y, GLfloat z, const char* text);
    void calculate_frequency();
    void calculate_rate();

    ALuint	buffer[NUM_BUFFERS];
    ALuint	source[NUM_SOURCES];
    ALuint  environment[NUM_ENVIRONMENTS];
    scenedescription *scene;

    
};

#endif // GLWIDGET_H
