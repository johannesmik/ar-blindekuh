#ifndef GLWIDGET_H
#define GLWIDGET_H




#include <QGLWidget>
#include <QTimer>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer timer;
    void print_text(GLfloat x, GLfloat y, GLfloat z, char* text);

    
};

#endif // GLWIDGET_H
