#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QDateTime>
#include "mymarkertracker.h"
#include "poseestimation.h"

MyMarkerTracker::MyMarkerTracker(QObject *parent):
    QObject(parent),
    cap(0)
{
    if(!cap.isOpened()){
        cap.open("../MarkerMovie.mpg");
        std::cout << std::endl << "Cannot open Webcam, using Movie-File" << std::endl;
        if(!cap.isOpened()){
            std::cout << std::endl << "Cannot open Movie-File, exiting" << std::endl;
            exit(0);
        }
    }

    //use a higher resolution
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

void MyMarkerTracker::queryForMarker()
{

    // get a new frame from camera
    if(!cap.read(frame)){
        qDebug() << "disconntected or end of file!";
        return;
    }
    emit(frameUpdate(frame));

    cv::cvtColor(frame, bw, CV_BGR2GRAY);
    IplImage bw_ipl(bw);
    cv::threshold(bw, adaptivethreshold, 190.0, 255.0, cv::THRESH_BINARY|cv::THRESH_OTSU);
//        cv::cvtColor(adaptivethreshold, test, CV_GRAY2BGR);
//        emit(frameUpdate(test));

    cv::findContours(adaptivethreshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    if(contours.size() == 0){
        qDebug() << "invalid frame"; // when the camera is no yet initialized, we get a timeout, so just wait for next frame
        return;
    }

    // this data structure is send via a signal to the opengl threads
    std::vector<QPair<std::vector<float>, int> > result;

    //iterate over all found contours
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
    {
        std::vector<cv::Point> approxResult;
        cv::approxPolyDP(contours[idx], approxResult, 7.68789, true);

        cv::Rect bound = boundingRect(approxResult);
        if(approxResult.size() != 4 || bound.area() < 30 || bound.height > frame.size().height - 10 || bound.width > frame.size().width - 10){
            //We need 4 corners, the area has to be large enough, but not too large
            continue;
        }
        int halfStripeLength;

        std::vector<cv::Vec4f> fittedLines;
        // for all corner points of approximated quadrilateral
        for(int p = 0; p<4; p++){
            // number of line segments
            int lscount = 7;

            // difference vector for this edge of the contour
            const cv::Point2f diff(approxResult[(p+1)%4].x - approxResult[p].x, approxResult[(p+1)%4].y - approxResult[p].y);
            const double edgeLength = sqrt(diff.x * diff.x + diff.y * diff.y); //length of this edge of the approximated rectangle in pixels

            // this is the vector parallel to the edge, normalized to a vector length of 1
            // we will take 3 pixels in this direction
            const cv::Point2f directionParallel(diff.x / edgeLength, diff.y / edgeLength);

            // this is the vector orthogonal to the edge, normalized to a vector length of 1
            // we will take stripelength*2+1 pixels in this direction
            const cv::Point2f directionOrthogonal(directionParallel.y, directionParallel.x*-1);

            // calculate the length of our stripe and create it
            halfStripeLength = 0.8*edgeLength/lscount;
            if (halfStripeLength < 2) halfStripeLength = 2;
            cv::Mat stripe(halfStripeLength*2+1, 3, CV_8UC1);

            double *sobelValues = new double[halfStripeLength*2-1];
            std::vector<cv::Point2f> exactPoints;

            //for every line segment on this edge, do not handle the 0th linesegment (would be on the corner)
            for(int ls = 1; ls <lscount; ls++){
                //first, get edge delimiter coordinates
                const cv::Point edgeDelimiter(approxResult[p].x + diff.x * ls / lscount,
                                          approxResult[p].y + diff.y * ls / lscount);

                // fill stripe from bw image
                for(int i = -1; i<=1; i++){
                    for(int j = -halfStripeLength; j<=halfStripeLength; j++){
                        cv::Point_<double> subPixel((double)edgeDelimiter.x + ((double)i*directionParallel.x) + ((double)j*directionOrthogonal.x),
                                                (double)edgeDelimiter.y + ((double)i*directionParallel.y) + ((double)j*directionOrthogonal.y));
                        stripe.at<uchar>(halfStripeLength + j, 1+i) = subpixSampleSafe(&bw_ipl, subPixel);
                    }
                }

                //apply sobel operator to stripe
                // ( -1 , -2, -1 )
                // (  0 ,  0,  0 )
                // (  1 ,  2,  1 )

                //at the same time, extract maximum Value from sobelValues
                double max = -std::numeric_limits<double>::max();
                int maxIndex = 0;
                for(int i = 1; i<halfStripeLength*2; i++)
                {
                    const uchar *row1 = stripe.ptr<uchar>(i-1);
                    const uchar *row3 = stripe.ptr<uchar>(i+1);
                    sobelValues[i-1] = -row1[0] - 2 * row1[1] - row1[2] +
                                        row3[0] + 2 * row3[1] + row3[2];
                    if(sobelValues[i-1]>max){
                        max = sobelValues[i-1];
                        maxIndex = i-1;
                    }
                }
                double y0,y1,y2; // y0 .. y1 .. y2
                y0 = (maxIndex <= 0) ? 0 : sobelValues[maxIndex-1];
                y1 = sobelValues[maxIndex];
                y2 = (maxIndex >= halfStripeLength*2-2) ? 0 : sobelValues[maxIndex+1];

                double pos = (y2 - y0) / (4*y1 - 2*y0 - 2*y2 );
                if (std::isinf(pos)) {
                        // value of pos is infinity, just don't consider this point for further calculations
                        continue;
                }

                int maxIndexShift = maxIndex - halfStripeLength;
                cv::Point2f exactPoint((double)edgeDelimiter.x + ((double)maxIndexShift + pos)*directionOrthogonal.x,
                                             (double)edgeDelimiter.y + ((double)maxIndexShift + pos)*directionOrthogonal.y);

                // store this exact point in a list
                exactPoints.push_back(exactPoint);

            }

            // use the list of exact points to fit a line
            cv::Vec4f fittedLine;
            cv::fitLine(exactPoints, fittedLine, CV_DIST_L2, 0, 0.01, 0.01);

            // and store this line in a list
            fittedLines.push_back(fittedLine);

            delete sobelValues;
        } // end iterate over corners of quadrilateral

        std::vector<cv::Point2f> exactCornerPoints;

        // iterate over all lines to calculate exact corner points
        //
        // a + l*c = b + k*d
        //
        // (I)  l = (b_x - a_x + k*d_x)/c_x
        // (II) k = (a_y - b_y + l*c_y)/d_y
        //
        // solve for l:
        // wolframalpha (renamed variables): solve a+x*c -b - y*d = 0, e + x*g - f - y*h = 0 for x and y:
        //
        // l = (d_y*(a_x - b_x)+d_x*(b_y-a_y))/(d_x*c_y - c_x*d_y)
        for(int I = 0; I<4; I++){
            //draw line
//                    cv::Point pt1, pt2;
//                    pt1.x = fittedLines[I][2] - 300 * fittedLines[I][0];
//                    pt1.y = fittedLines[I][3] - 300 * fittedLines[I][1];
//                    pt2.x = pt1.x + 500 * fittedLines[I][0];
//                    pt2.y = pt1.y + 500 * fittedLines[I][1];

//                    line(frame, pt1, pt2, CV_RGB(255,0,0), 1);
            const double c_x = fittedLines[I][0];
            const double c_y = fittedLines[I][1];
            const double a_x = fittedLines[I][2];
            const double a_y = fittedLines[I][3];
            int II = (I+1)%4;
            const double d_x = fittedLines[II][0];
            const double d_y = fittedLines[II][1];
            const double b_x = fittedLines[II][2];
            const double b_y = fittedLines[II][3];
            const double l = (d_y*(a_x - b_x)+d_x*(b_y-a_y))/(d_x*c_y - c_x*d_y);
            cv::Point2f exactCornerPoint(a_x + l*c_x, a_y + l*c_y);

            // store exact point in a list
            exactCornerPoints.push_back(exactCornerPoint);
        }
        // we map to the center of our pixels, therefore *.5
        cv::Point2f resultCoords[] = {cv::Point2f(-0.5, -0.5), cv::Point2f(5.5, -0.5), cv::Point2f(5.5, 5.5), cv::Point2f(-0.5, 5.5)};

        cv::Mat transformationMatrix = cv::getPerspectiveTransform(exactCornerPoints.data(), resultCoords);

        // create marker buffer
        cv::Mat marker(6, 6, frame.type());

        // and fill it from original frame
        cv::warpPerspective(frame, marker, transformationMatrix, cv::Size(6,6));

        //convert to grayscale
        cvtColor(marker, marker, CV_BGR2GRAY);

        // and apply a automat
        cv::threshold(marker, marker, 127.0, 255.0, cv::THRESH_BINARY);

        if(!isMarker(marker)){
            continue;
        }

        // highlight detected markers in image
        cv::Point *points = approxResult.data();
        int size = approxResult.size();
        cv::polylines(frame, (const cv::Point**)&points, &size, 1, true,  CV_RGB(0,255,0), 2);

        // calculate id for each position, the overhead is not so much
        ushort idPos[4] = {0,0,0,0};
        for(int i = 1; i<5; i++){
            for(int j = 1; j<5; j++){
                if(marker.at<uchar>(i, j) == 0){
                    idPos[0] |= 1<<(4*(i-1)+(j-1));
                    idPos[1] |= 1<<((i-1)+4*(3-(j-1)));
                    idPos[2] |= 1<<(4*(3-(i-1))+(3-(j-1)));
                    idPos[3] |= 1<<((3-(i-1))+4*(j-1));
                }
            }
        }

        // get the minimal id and store rotationIndex
        ushort id = idPos[0];
        int rotationIndex = 0;
        for(int i = 1; i<4; i++){
            if(idPos[i] < id) {
                rotationIndex = i;
                id =  idPos[i];
            }
        }

        //create 4x4 transformationMatrix
        std::vector<float> transformationMatrixOpenGL;
        transformationMatrixOpenGL.resize(16);

        // rotate corner points according to rotationIndex and move origin to center of image
        CvPoint2D32f shiftedCornerPoints[4];
        for(int i = 0; i<4; i++){
            int shiftedIndex = (i+rotationIndex) & 3; // & 3 (11 in binary format)is the same as % 4, but faster
            shiftedCornerPoints[i].x = exactCornerPoints[shiftedIndex].x - (frame.size().width >> 1);
            shiftedCornerPoints[i].y = -(exactCornerPoints[shiftedIndex].y - (frame.size().height >> 1));
        }

        //rotate corners and transform coordinates
        estimateSquarePose(transformationMatrixOpenGL.data(), shiftedCornerPoints, 0.045);
        qDebug() << QDateTime::currentMSecsSinceEpoch() << "detected marker: " << id;
        result.push_back(QPair<std::vector<float>, int>(transformationMatrixOpenGL, id));

    } // end of iteration over contours
    emit(markerPositionUpdate(result));
}


int MyMarkerTracker::subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p )
{
        int x = int( floorf ( p.x ) );
        int y = int( floorf ( p.y ) );

        if ( x < 0 || x >= pSrc->width  - 1 ||
                 y < 0 || y >= pSrc->height - 1 )
                return 127;

        int dx = int ( 256 * ( p.x - floorf ( p.x ) ) );
        int dy = int ( 256 * ( p.y - floorf ( p.y ) ) );

        unsigned char* i = ( unsigned char* ) ( ( pSrc->imageData + y * pSrc->widthStep ) + x );
        int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
        i += pSrc->widthStep;
        int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
        return a + ( ( dy * ( b - a) ) >> 8 );
}

// check if marker really is a marker
bool MyMarkerTracker::isMarker(const cv::Mat &marker)
{
    bool realMarker = false;

    //there has to be at least one white part inside the marker
    for(int i = 1; i<5 && !realMarker; i++){
        for(int j = 1; j<5 && !realMarker; j++){
            if(marker.at<uchar>(i, j) == 255){
                realMarker = true;
            }
        }
    }

    // all the pixels on the edge have to be black
    for(int i = 0; i<6 && realMarker; i++){
        if(marker.at<uchar>(0, i) != 0 || marker.at<uchar>(5, i) != 0 || marker.at<uchar>(i, 0) != 0 || marker.at<uchar>(i, 5) != 0){
            realMarker = false;
        }
    }

    return realMarker;
}
