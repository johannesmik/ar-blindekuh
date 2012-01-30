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
        CVWidget(QWidget *parent=NULL);
        ~CVWidget();

private:
      CvMemStorage *storage; // Image storage
      CvMemStorage *memStorage;
      CvCapture *capture;
      _IplImage *cv_image;
      IplImage *iplConverted;
      IplImage *iplThreshold;

      bool isFirstStripe;
      bool isFirstMarker;

      QImage q_image;
      QTimer *q_timer;

      void paintEvent(QPaintEvent *e);
      void resize(int w, int h);

      int subpixSampleSafe(const IplImage *pSrc, CvPoint2D32f p);
      bool isMarker(const cv::Mat &marker);

  public slots:
      void queryFrame();

};

#endif // CVWIDGET_H
