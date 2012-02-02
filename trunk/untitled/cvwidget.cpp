#include "cvwidget.h"


CVWidget::CVWidget(QWidget *parent):QWidget(parent)
{
}

/** \brief Destructor
 *
 * This function releases the capture from the webcam by calling
 * the proper OpenCV function.
 */
CVWidget::~CVWidget()
{

}

void CVWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    //qDebug("q_image.height(): %i this->height(): %i", q_image.height(), this->height());
    QPainter painter(this);
    //do not scale, as data is copied when scaled
    //painter.drawImage(0,0, frameImage.scaledToHeight(this->height()) );
    painter.drawImage(0,0, frameImage);

}

void CVWidget::frameUpdate(const cv::Mat& frame)
{
    frameImage = QImage(frame.data, frame.size().width, frame.size().height, QImage::Format_RGB888);
    update();
}


