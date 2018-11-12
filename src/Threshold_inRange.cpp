#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

/** Function Headers */
void on_low_r_thresh_trackbar(int, void *);
void on_high_r_thresh_trackbar(int, void *);
void on_low_g_thresh_trackbar(int, void *);
void on_high_g_thresh_trackbar(int, void *);
void on_low_b_thresh_trackbar(int, void *);
void on_high_b_thresh_trackbar(int, void *);

/** Global Variables */
int low_r=136, low_g=231, low_b=96;
int high_r=255, high_g=255, high_b=255;


/**
 * @function thresh_callback
 */

int thresh = 40;
int max_thresh = 255;
RNG rng(12345);
int nbGraine=0;
 
void thresh_callback(int, void*,Mat src)
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  
  Canny( src, canny_output, thresh, thresh*2, 3 );	/// Detect edges using canny
  
  findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );  /// Find contours

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       nbGraine++;
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}


/** @function main */
int main()
{
    //! [mat]
    Mat frame, frame_threshold, frame_morph;
    Mat element = getStructuringElement( MORPH_RECT, Size( 4, 4 ), Point(-1,-1) );
    
    //! [mat]
    //! [cap]
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,800);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);	//768
    
    
    //! [cap]
    //! [window]
    namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Object Detection", WINDOW_NORMAL);
    namedWindow("ThreShold Result", WINDOW_AUTOSIZE);
    //! [window]
    //! [trackbar]
    //-- Trackbars to set thresholds for RGB values
  /*createTrackbar("Low R","Object Detection", &low_r, 255, on_low_r_thresh_trackbar);
    createTrackbar("High R","Object Detection", &high_r, 255, on_high_r_thresh_trackbar);
    createTrackbar("Low G","Object Detection", &low_g, 255, on_low_g_thresh_trackbar);
    createTrackbar("High G","Object Detection", &high_g, 255, on_high_g_thresh_trackbar);
    createTrackbar("Low B","Object Detection", &low_b, 255, on_low_b_thresh_trackbar);
    createTrackbar("High B","Object Detection", &high_b, 255, on_high_b_thresh_trackbar);
  */
	
  //! [trackbar]
    while((char)waitKey(1)!='q'){
        //! [while]
        cap>>frame;
        if(frame.empty())
            break;
        //-- Detect the object based on RGB Range Values
        inRange(frame,Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);	//thresholding
        morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
        //! [while]
        //! [show]
        //-- Show the frames
        
        thresh_callback( 0, 0,frame_morph);
        //imshow("Video Capture",frame);
        imshow("ThreShold Result after noise filtering",frame_morph);
        //! [show]
    }
    imwrite("tresholdedimage.jpg",frame_threshold);
    imwrite("THRES_MORHImage5_5.jpg",frame_morph);
    return 0;
}
//! [low]
/** @function on_low_r_thresh_trackbar */
void on_low_r_thresh_trackbar(int, void *)
{
    low_r = min(high_r-1, low_r);
    setTrackbarPos("Low R","Object Detection", low_r);
}
//! [low]
//! [high]
/** @function on_high_r_thresh_trackbar */
void on_high_r_thresh_trackbar(int, void *)
{
    high_r = max(high_r, low_r+1);
    setTrackbarPos("High R", "Object Detection", high_r);
}
//![high]
/** @function on_low_g_thresh_trackbar */
void on_low_g_thresh_trackbar(int, void *)
{
    low_g = min(high_g-1, low_g);
    setTrackbarPos("Low G","Object Detection", low_g);
}

/** @function on_high_g_thresh_trackbar */
void on_high_g_thresh_trackbar(int, void *)
{
    high_g = max(high_g, low_g+1);
    setTrackbarPos("High G", "Object Detection", high_g);
}

/** @function on_low_b_thresh_trackbar */
void on_low_b_thresh_trackbar(int, void *)
{
    low_b= min(high_b-1, low_b);
    setTrackbarPos("Low B","Object Detection", low_b);
}

/** @function on_high_b_thresh_trackbar */
void on_high_b_thresh_trackbar(int, void *)
{
    high_b = max(high_b, low_b+1);
    setTrackbarPos("High B", "Object Detection", high_b);
}
