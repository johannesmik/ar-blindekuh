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
    LIBS += -L"C:\Program Files\freeglut-2.4.0\lib"

    # OpenCv
    #LIBS += -L"C:\OpenCV2.1\lib" -lcv210 -lhighgui210 -lcxcore210
    LIBS += -lopencv_core231 -lopencv_highgui231 -lopencv_imgproc231 -L"D:\blindekuh\opencv\opencv\build\x86\vc9\lib"
    INCLUDEPATH += "C:\OpenCV2.1\include"
    INCLUDEPATH += "C:\OpenCV2.1\bin"
    INCLUDEPATH += "D:\blindekuh\opencv\opencv\build\include"
    INCLUDEPATH += "D:\blindekuh\freealut-1.1.0-bin\freealut-1.1.0-bin\include"
    INCLUDEPATH += "C:\Program Files\freeglut-2.4.0\include"

    # OpenAl and Alut
    INCLUDEPATH += "C:\Program Files\freealut\include"
    INCLUDEPATH += "C:\Program Files\OpenAL 1.1 SDK\include"
    LIBS += -L"C:\Program Files\freealut\lib"
    LIBS += -L"C:\Program Files\OpenAL 1.1 SDK\libs\Win32"
    LIBS += -L"D:\blindekuh\freealut-1.1.0-bin\freealut-1.1.0-bin\lib"
    LIBS += -lOpenAL32 -lalut
} else {
    LIBS += -lglut -lopencv_core -lopencv_highgui -lopencv_imgproc -lopenal -lGLU -lalut
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
    mymarkertracker.cpp \
    soundoutput.cpp \
    scenedescription.cpp \
    gamelogic.cpp \
    highscore.cpp \
    marker.cpp \
    myapplication.cpp

HEADERS  += mainwindow.h \
        glwidget.h \
        cvwidget.h \
    highscorewidget.h \
    poseestimation.h \
    mymarkertracker.h \
    soundoutput.h \
    scenedescription.h \
    gamelogic.h \
    highscore.h \
    marker.h \
    myapplication.h

FORMS    += mainwindow.ui
