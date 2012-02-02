#-------------------------------------------------
#
# Project created by QtCreator 2012-01-24T23:29:16
#
#-------------------------------------------------


# QT
QT       += core gui opengl

win32 {
# Freeglut
    INCLUDEPATH += "C:\Program Files\freeglut28\include"
    LIBS += -lfreeglut -L"C:\Program Files\freeglut28\lib"

    # OpenCv
    LIBS += -L"C:\OpenCV2.1\lib"
    LIBS += -lcv210 -lhighgui210 -lcxcore210
    INCLUDEPATH += "C:\OpenCV2.1\include"
    INCLUDEPATH += "C:\OpenCV2.1\bin"

    # OpenAl and Alut
    INCLUDEPATH += "C:\Program Files\freealut\include"
    INCLUDEPATH += "C:\Program Files\OpenAL 1.1 SDK\include"
    LIBS += -L"C:\Program Files\freealut\lib"
    LIBS += -L"C:\Program Files\OpenAL 1.1 SDK\libs\Win32"
    LIBS += -lOpenAL32 -lalut
} else {
    LIBS += -lglut -lopencv_core -lopencv_highgui -lGLU -lalut
}




#------------#
TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        glwidget.cpp \
        cvwidget.cpp \
    highscorewidget.cpp \
    poseestimation.cpp \
    mymarkertracker.cpp

HEADERS  += mainwindow.h \
        glwidget.h \
        cvwidget.h \
    highscorewidget.h \
    poseestimation.h \
    mymarkertracker.h

FORMS    += mainwindow.ui
