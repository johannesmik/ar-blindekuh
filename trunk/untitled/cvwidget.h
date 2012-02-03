#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <QtOpenGL>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "poseestimation.h"

class QTimer;
struct CvCapture;
struct _IplImage;

class CVWidget: public QWidget
{
        Q_OBJECT;

public:
        CVWidget(QWidget *parent=0);
        ~CVWidget();

protected:
        virtual void paintEvent(QPaintEvent *e);

public slots:
      void frameUpdate(const cv::Mat& frame);
      void frameUpdateBw(const cv::Mat& bwFrame);

private:
      QImage frameImage;
      QImage frameImageBw;


};

#endif // CVWIDGET_H
