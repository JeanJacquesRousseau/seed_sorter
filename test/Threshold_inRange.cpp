#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <stdlib.h>


using namespace std;
using namespace cv;

/** Function Headers */
/** Global Variables */



/**
 * @function thresh_callback
 */
 
Mat src_gray;
int thresh = 140;
int max_thresh = 255;
RNG rng(12345);
int nbGraine=0;
 
void thresh_callback(int, void*,Mat src)//Find contours
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

Mat find_moments( Mat gray )	//Find centroid
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
 
    /// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 );
    // Find contours
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
 
    /// Get the moments
    vector<Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }
 
    ///  Get the centroid of figures.
    vector<Point2f> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
    
 
    /// Draw contours
    
    Mat drawing(canny_output.size(), CV_8UC3, Scalar(255,255,255));

    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(167,151,0);
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
        circle( drawing, mc[i], 4, Scalar(0,0,255), -1, 7, 0 );
    }
 
    /// Show the resultant image
    //namedWindow( "Contours", WINDOW_AUTOSIZE );
    //imshow( "Contours", drawing );
    return drawing;
    
		
}



/** @function main */
int main()
{
    //! [mat]
    Mat frame, frame_morph, frame_gray, frame_threshold, frame_label;
    Mat element = getStructuringElement( MORPH_RECT, Size( 6, 6 ), Point(-1,-1) );
    
    //! [cap]
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,800);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);	//768
    
    
    //VideoWriter video1("source.avi",CV_FOURCC('M','J','P','G'),10, Size(800,600));
    VideoWriter video("centroids.avi",CV_FOURCC('M','J','P','G'),2, Size(800,600));
    
 /* 
    
    //! [window]
    namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Object Detection", WINDOW_NORMAL);
    namedWindow("ThreShold Result", WINDOW_AUTOSIZE);

    
*/
	
 
    while((char)waitKey(1)!='q'){
        
        cap>>frame;
        if(frame.empty()) break;
        
        
        //-- Detect the object based on RGB Range Values
        cvtColor( frame, frame_gray, COLOR_BGR2GRAY );	//Grey scale convertion
        threshold(frame_gray, frame_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);  //Otsu thresholding
        morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
		frame_label = find_moments( frame_morph);	// Contours/Centroid
		//blur( src_gray, src_gray, Size(3,3) );
        //cethresh_callback( 0, 0,frame_morph);
        
        
        //-- [show]
        imshow("Video Capture",frame);
        imshow("Threshold before noise filtering",frame_label);
        //imshow("ThreShold Result after noise filtering",frame_morph);        
        //video.write(frame);
        //video1.write(frame_label);
    }
    //imwrite("tresholdedimage.jpg",frame_threshold);
    //imwrite("Centroids.jpg",frame_morph);
    cap.release();
    //video.release();
    //video1.release();
    return 0;
}

