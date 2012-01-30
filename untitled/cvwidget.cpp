#include "cvwidget.h"


CVWidget::CVWidget(QWidget *parent):QWidget(parent)
{

    capture =  cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {

        QMessageBox::information(this, "Webcam", "Es wurde keine Webcam gefunden", "Ok");

    }


    //grab one frame to get width and height
    IplImage* frame = cvQueryFrame( capture );

    q_image = QImage(QSize(frame->width, frame->height), QImage::Format_RGB888);

    // Set cv_image headers and share buffer with q_image
    cv_image = cvCreateImageHeader(cvSize(q_image.width(), q_image.height()),8,3);
    qDebug("Width: %i Height %i", q_image.width(), q_image.height());
    cv_image->imageData = (char*)q_image.bits(); //share buffer

    //... flip or copy?
    cvCopy(frame, cv_image, 0);

    // cv frames come in BGR but shared buffer should be RGB
    cvCvtColor(cv_image, cv_image, CV_BGR2RGB);

    // set up cv-storage
    storage = cvCreateMemStorage(0);
    memStorage = cvCreateMemStorage(0);


    // Connect Timer to queryFrame
    q_timer = new QTimer(this);
    connect(q_timer, SIGNAL(timeout()), this, SLOT(queryFrame()));
    q_timer->start(50);

}

/** \brief Destructor
 *
 * This function releases the capture from the webcam by calling
 * the proper OpenCV function.
 */
CVWidget::~CVWidget()
{
    // Release


    cvClearMemStorage(storage);
    cvReleaseMemStorage( &storage);
    cvReleaseCapture( &capture );

}

void CVWidget::paintEvent(QPaintEvent *e)
{


    //qDebug("q_image.height(): %i this->height(): %i", q_image.height(), this->height());
    QPainter painter(this);
    painter.drawImage(0,0, q_image.scaledToHeight(this->height()) );

}



void CVWidget::queryFrame()
{

    IplImage *frame = cvQueryFrame( capture );

    //cvCopy( frame, cv_image, 0);
    cvCvtColor(frame, frame, CV_BGR2RGB);
    //cvCircle (cv_image, cvPoint(50,50), 10, CV_RGB(0,255,0), -1);



      //// MARKER DETECTION START

        isFirstStripe = true;
        isFirstMarker = true;
        CvSize picSize = cvGetSize(frame);
        CvSeq* contours;

        iplConverted = cvCreateImage(picSize, IPL_DEPTH_8U, 1);
        iplThreshold = cvCreateImage(picSize, IPL_DEPTH_8U, 1);

        cvConvertImage(frame, iplConverted, 0);
        cvThreshold(iplConverted, iplThreshold, 190, 255, CV_THRESH_BINARY);

        // Find Contours and iterate over them
        cvFindContours(iplThreshold, memStorage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

        for (; contours; contours = contours->h_next)
        {

            CvSeq* result = cvApproxPoly(contours, sizeof(CvContour), memStorage, CV_POLY_APPROX_DP,cvContourPerimeter(contours)*0.02, 0);
            CvRect r = cvBoundingRect(result);

            if (r.height < 20 || r.width < 20) {
                continue;
            }

            if (result->total==4)
            {
                int count = 4;
                CvPoint *rect = new CvPoint[4];
                cvCvtSeqToArray(result, rect);
                cvPolyLine(frame, &rect, &count, 1, 1, CV_RGB(255,0,0), 2);

                float lineParams[16];

                for (int i=0; i<4; ++i)
                {
                    cvCircle (frame, rect[i], 3, CV_RGB(0,255,0), -1);

                    double dx = (double)(rect[(i+1)%4].x-rect[i].x)/7.0;
                    double dy = (double)(rect[(i+1)%4].y-rect[i].y)/7.0;

                    int stripeLength = (int)(0.8*sqrt (dx*dx+dy*dy));
                    if (stripeLength < 5)
                    stripeLength = 5;

                    //make stripeLength odd (because of the shift in nStop)
                    stripeLength |= 1;

                    //e.g. stripeLength = 5 --> from -2 to 2
                    int nStop  = stripeLength>>1;
                    int nStart = -nStop;

                    CvSize stripeSize;
                    stripeSize.width = 3;
                    stripeSize.height = stripeLength;

                    CvPoint2D32f stripeVecX;
                    CvPoint2D32f stripeVecY;

                    //normalize vectors
                    double diffLength = sqrt ( dx*dx+dy*dy );
                    stripeVecX.x = dx / diffLength;
                    stripeVecX.y = dy / diffLength;

                    stripeVecY.x =  stripeVecX.y;
                    stripeVecY.y = -stripeVecX.x;

                    IplImage* iplStripe = cvCreateImage( stripeSize, IPL_DEPTH_8U, 1 );

                    // Array for edge point centers
                    CvPoint2D32f points[6];

                    for (int j=1; j<7; ++j)
                    {
                        double px = (double)rect[i].x+(double)j*dx;
                        double py = (double)rect[i].y+(double)j*dy;

                        CvPoint p;
                        p.x = (int)px;
                        p.y = (int)py;
                        cvCircle (frame, p, 2, CV_RGB(0,0,255), -1);

                        for ( int m = -1; m <= 1; ++m )
                        {
                            for ( int n = nStart; n <= nStop; ++n )
                            {
                                CvPoint2D32f subPixel;

                                subPixel.x = (double)p.x + ((double)m * stripeVecX.x) + ((double)n * stripeVecY.x);
                                subPixel.y = (double)p.y + ((double)m * stripeVecX.y) + ((double)n * stripeVecY.y);

                                CvPoint p2;
                                p2.x = (int)subPixel.x;
                                p2.y = (int)subPixel.y;

                                if (isFirstStripe)
                                    cvCircle (frame, p2, 1, CV_RGB(255,0,255), -1);
                                else
                                    cvCircle (frame, p2, 1, CV_RGB(0,255,255), -1);

                                int pixel = subpixSampleSafe (iplConverted, subPixel);

                                int w = m + 1; //add 1 to shift to 0..2
                                int h = n + ( stripeLength >> 1 ); //add stripelenght>>1 to shift to 0..stripeLength

                                *(iplStripe->imageData + h * iplStripe->widthStep  + w) =  pixel; //set pointer to correct position and safe subpixel intensity
                            }
                        }

                        //use sobel operator on stripe
                        // ( -1 , -2, -1 )
                        // (  0 ,  0,  0 )
                        // (  1 ,  2,  1 )

                        double* sobelValues = new double[stripeLength-2];
                        for (int n = 1; n < (stripeLength-1); n++)
                        {
                            unsigned char* stripePtr = ( unsigned char* )( iplStripe->imageData + (n-1) * iplStripe->widthStep );
                            double r1 = -stripePtr[ 0 ] - 2 * stripePtr[ 1 ] - stripePtr[ 2 ];

                            stripePtr += 2*iplStripe->widthStep;
                            double r3 =  stripePtr[ 0 ] + 2 * stripePtr[ 1 ] + stripePtr[ 2 ];
                            sobelValues[n-1] = r1+r3;
                        }

                        double maxVal = -1;
                        int maxIndex = 0;
                        for (int n=0; n<stripeLength-2; ++n)
                        {
                            if ( sobelValues[n] > maxVal )
                            {
                                maxVal = sobelValues[n];
                                maxIndex = n;
                            }
                        }

                        double y0,y1,y2; // y0 .. y1 .. y2
                        y0 = (maxIndex <= 0) ? 0 : sobelValues[maxIndex-1];
                        y1 = sobelValues[maxIndex];
                        y2 = (maxIndex >= stripeLength-3) ? 0 : sobelValues[maxIndex+1];

                        //formula for calculating the x-coordinate of the vertex of a parabola, given 3 points with equal distances
                        //(xv means the x value of the vertex, d the distance between the points):
                        //xv = x1 + (d / 2) * (y2 - y0)/(2*y1 - y0 - y2)

                        double pos = (y2 - y0) / (4*y1 - 2*y0 - 2*y2 ); //d = 1 because of the normalization and x1 will be added later

                        if (pos!=pos) { // check for infinity or NaN
                            continue;
                        }

                        CvPoint2D32f edgeCenter; //exact point with subpixel accuracy
                        int maxIndexShift = maxIndex - (stripeLength>>1);

                        //shift the original edgepoint accordingly
                        edgeCenter.x = (double)p.x + (((double)maxIndexShift+pos) * stripeVecY.x);
                        edgeCenter.y = (double)p.y + (((double)maxIndexShift+pos) * stripeVecY.y);

                        CvPoint p_tmp;
                        p_tmp.x = (int)edgeCenter.x;
                        p_tmp.y = (int)edgeCenter.y;
                        cvCircle (frame, p_tmp, 1, CV_RGB(0,0,255), -1);

                        points[j-1].x = edgeCenter.x;
                        points[j-1].y = edgeCenter.y;


                    } // end of loop over edge points of one edge

                    cvReleaseImage ( &iplStripe );

                    // we now have the array of exact edge centers stored in "points"
                    CvMat mat = cvMat ( 1, 6, CV_32FC2, points);
                    cvFitLine ( &mat, CV_DIST_L2, 0, 0.01, 0.01, &lineParams[4*i] );
                    // cvFitLine stores the calculated line in lineParams in the following way:
                    // vec.x, vec.y, point.x, point.y

                    CvPoint p;
                    p.x=(int)lineParams[4*i+2] - (int)(50.0*lineParams[4*i+0]);
                    p.y=(int)lineParams[4*i+3] - (int)(50.0*lineParams[4*i+1]);

                    CvPoint p2;
                    p2.x = (int)lineParams[4*i+2] + (int)(50.0*lineParams[4*i+0]);
                    p2.y = (int)lineParams[4*i+3] + (int)(50.0*lineParams[4*i+1]);

                    cvLine (frame, p, p2, CV_RGB(0,255,255), 1, 8, 0);

                } // end of loop over the 4 edges

                // so far we stored the exact line parameters and show the lines in the image
                // now we have to calculate the exact corners
                CvPoint2D32f corners[4];

                for (int i=0; i<4; ++i)
                {
                    int j = (i+1)%4;
                    double x0,x1,y0,y1,u0,u1,v0,v1;
                    x0 = lineParams[4*i+2]; y0 = lineParams[4*i+3];
                    x1 = lineParams[4*j+2]; y1 = lineParams[4*j+3];

                    u0 = lineParams[4*i+0]; v0 = lineParams[4*i+1];
                    u1 = lineParams[4*j+0]; v1 = lineParams[4*j+1];

                    // (x|y) = p + s * vec
                    // s = Ds / D (see cramer's rule)
                    // (x|y) = p + (Ds / D) * vec
                    // (x|y) = (p * D / D) + (Ds * vec / D)
                    // (x|y) = (p * D + Ds * vec) / D
                    // (x|y) = a / c;
                    double a =  x1*u0*v1 - y1*u0*u1 - x0*u1*v0 + y0*u0*u1;
                    double b = -x0*v0*v1 + y0*u0*v1 + x1*v0*v1 - y1*v0*u1;
                    double c =  v1*u0-v0*u1;

                    if ( fabs(c) < 0.001 ) //lines parallel?
                    {
                        continue;
                    }

                    a /= c;
                    b /= c;

                    //exact corner
                    corners[i].x = a;
                    corners[i].y = b;
                    CvPoint p;
                    p.x = (int)corners[i].x;
                    p.y = (int)corners[i].y;

                    cvCircle (frame, p, 5, CV_RGB(255,255,0), -1);
                } //finished the calculation of the exact corners

                CvPoint2D32f targetCorners[4];
                targetCorners[0].x = -0.5; targetCorners[0].y = -0.5;
                targetCorners[1].x =  5.5; targetCorners[1].y = -0.5;
                targetCorners[2].x =  5.5; targetCorners[2].y =  5.5;
                targetCorners[3].x = -0.5; targetCorners[3].y =  5.5;

                //create and calculate the matrix of perspective transform
                CvMat* projMat = cvCreateMat (3, 3, CV_32F );
                cvWarpPerspectiveQMatrix ( corners, targetCorners, projMat);

                //create image for the marker
                CvSize markerSize;
                markerSize.width  = 6;
                markerSize.height = 6;
                IplImage* iplMarker = cvCreateImage( markerSize, IPL_DEPTH_8U, 1 );

                //change the perspective in the marker image using the previously calculated matrix
                cvWarpPerspective(iplConverted, iplMarker, projMat, CV_WARP_FILL_OUTLIERS,  cvScalarAll(0));

                cvThreshold(iplMarker, iplMarker, 190, 255, CV_THRESH_BINARY);

                // check if border is black
                int code = 0;
                for (int i = 0; i < 6; ++i)
                {
                    int pixel1 = ((unsigned char*)(iplMarker->imageData + 0*iplMarker->widthStep + i))[0]; //top
                    int pixel2 = ((unsigned char*)(iplMarker->imageData + 5*iplMarker->widthStep + i))[0]; //bottom
                    int pixel3 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep))[0]; //left
                    int pixel4 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep + 5))[0]; //right
                    if ( ( pixel1 > 0 ) || ( pixel2 > 0 ) || ( pixel3 > 0 ) || ( pixel4 > 0 ) )
                    {
                        code = -1;
                        break;
                    }
                }

                if ( code < 0 ) continue;

                //copy the BW values into cP
                int cP[4][4];
                for ( int i=0; i < 4; ++i)
                {
                    for ( int j=0; j < 4; ++j)
                    {
                        cP[i][j] = ((unsigned char*)(iplMarker->imageData + (i+1)*iplMarker->widthStep + (j+1) ))[0];
                        cP[i][j] = (cP[i][j]==0) ? 1 : 0; //if black then 1 else 0
                    }
                }
                cvReleaseImage (&iplMarker);

                //save the ID of the marker
                int codes[4];
                codes[0] = codes[1] = codes[2] = codes[3] = 0;
                for (int i=0; i < 16; i++)
                {
                    int row = i>>2;
                    int col = i%4;

                    codes[0] <<= 1;
                    codes[0] |= cP[row][col]; // 0°

                    codes[1] <<= 1;
                    codes[1] |= cP[3-col][row]; // 90°

                    codes[2] <<= 1;
                    codes[2] |= cP[3-row][3-col]; // 180°

                    codes[3] <<= 1;
                    codes[3] |= cP[col][3-row]; // 270°
                }


                if ( (codes[0]==0) || (codes[0]==0xffff) ) {
                    qDebug("found empty or full marker!");
                    continue;
                }
                //account for symmetry
                code = codes[0];

                int angle = 0;

                for ( int i=1; i<4; ++i )
                {
                    if ( codes[i] < code )
                    {
                        code = codes[i];

                        angle = i;

                    }
                }


                //correct the order of the corners
                if(angle != 0)
                {
                    CvPoint2D32f corrected_corners[4];
                    for(int i = 0; i < 4; i++)	corrected_corners[(i + angle)%4] = corners[i];
                    for(int i = 0; i < 4; i++)	corners[i] = corrected_corners[i];
                }

                qDebug("Found: %04x\n", code);

                if ( isFirstMarker )
                {
                    //cvShowImage ( "Marker", iplMarker );
                    isFirstMarker = false;
                }


                // transfer camera coords to screen coords
                for(int i = 0; i < 4; i++)
                {
                    corners[i].x -= 160;
                    corners[i].y = -corners[i].y + 120;
                }

                float resultMatrix[16];
                estimateSquarePose( resultMatrix, corners, 0.045 );

                float x,y,z;
                x = resultMatrix[3];
                y = resultMatrix[7];
                z = resultMatrix[11];

                cvReleaseMat (&projMat);

                delete[] rect;
                } // end of if(result->total == 4)
            } // end of loop over contours



      //// MARKER DETECTION END

    cvConvertImage(frame, cv_image, 0);



    this->update();

    cvClearMemStorage(memStorage);
    cvReleaseImage (&iplConverted);
    cvReleaseImage (&iplThreshold);
}


int CVWidget::subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p ){

}

// check if marker really is a marker
bool CVWidget::isMarker(const cv::Mat &marker) {

}



