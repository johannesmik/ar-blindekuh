#ifndef MYMARKERTRACKER_H
#define MYMARKERTRACKER_H

#include <QThread>
#include <QDebug>
#include <opencv2/opencv.hpp>

class MyMarkerTracker : public QObject
{
    Q_OBJECT
public:
    explicit MyMarkerTracker(QObject *parent = 0);

signals:
    void markerPositionUpdate(std::vector<QPair<std::vector<float>, int> >);
    void frameUpdate(cv::Mat frame);

public slots:
    void queryForMarker();

private:
    int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p );
    bool isMarker(const cv::Mat &marker);

    cv::VideoCapture cap;
    cv::Mat bw;
    cv::Mat adaptivethreshold;
    cv::Mat test;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat frame;


};

#endif // MYMARKERTRACKER_H
