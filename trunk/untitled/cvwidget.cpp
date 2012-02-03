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
    //scaling is more expensive, but nicer
    QImage toPaint = frameImage.scaledToHeight(this->height()); //frameImage
    painter.drawImage(0,0, toPaint);
    painter.drawImage(toPaint.width(), 0, frameImageBw.scaledToHeight(this->height()) );


}

void CVWidget::frameUpdate(const cv::Mat& frame)
{
    frameImage = QImage(frame.data, frame.size().width, frame.size().height, QImage::Format_RGB888);
    update();
}

void CVWidget::frameUpdateBw(const cv::Mat& bwFrame)
{
    frameImageBw = QImage(bwFrame.data, bwFrame.size().width, bwFrame.size().height, QImage::Format_RGB888);
    update();
}

