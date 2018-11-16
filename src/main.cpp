/*
Author : David Patry 
*/
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace cv;

/** Function Headers */
/** Global Variables */
Mat src_gray;
int thresh = 140;
int max_thresh = 255;
RNG rng(12345);
uint32_t nbGraine=0;
uint32_t nccomps=0;

/**
 * @function thresh_callback
 */
 
Mat find_moments( Mat gray )	/// Find centroid
{	
	uint32_t i;
    Mat canny_output, labels, centroids, img_color, stats;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //namedWindow( "Debug Window", WINDOW_AUTOSIZE );
    //imshow( "Debug Window", canny_output);
    
    nccomps = cv::connectedComponentsWithStats (// Find # of blob
		gray, 
		labels,
		stats, 
		centroids
	);

	vector<cv::Vec3b> colors(nccomps+1);	/// Define vector of Vec3b # of blob size
	colors[0] = cv::Vec3b(0,0,0);	/// background pixels remain black.
	
	for( i = 1; i <= nccomps; i++ ) {	///Assign color in function of Area size of blob
		
		if( stats.at<int>(i, cv::CC_STAT_AREA) < 1250 ) colors[i] = cv::Vec3b(0,0,255);	///Soybeans in red
		else colors[i] = cv::Vec3b(0,255,0);	///Corn in green
    
		//if( stats.at<int>(i, cv::CC_STAT_AREA) < 100 ) colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black, looks useless for now. 
		//cout << "surface : " << stats.at<int>(i, cv::CC_STAT_AREA)<< "  couleur : " << colors[i] << endl;	//debug message
		//colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256); //Random color attribution exemple
	}
	
	
	/// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 ); 
    
    /// Find contours
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); 
 
    /// Get the moments
    vector<Moments> mu(contours.size() );
    for(uint32_t i = 0; i < contours.size(); i++ ){
		 mu[i] = moments(contours[i], false);
	}
 
    ///  Get the centroid of figures.
    vector<Point2f> mc(contours.size());
    for(uint32_t i = 0; i < contours.size(); i++ ){
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}
    
  
    /// Draw contours
    Mat drawing(canny_output.size(), CV_8UC3, Scalar(255,255,255));	///Set a blank image of same size as cannyoutput to write on
    Scalar color = Scalar(167,151,0);	/// Contours colors is teal ish
       
	for( int y = 0; y < drawing.rows; y++ )
		for( int x = 0; x < drawing.cols; x++ ){
			int label = labels.at<int>(y, x);	
			CV_Assert(0 <= label && label <= nccomps);
			drawing.at<cv::Vec3b>(y, x) = colors[label];
	}
    
	
    for( uint32_t i = 0; i< contours.size(); i++ ){
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, Scalar(255,0,0), -1, 7, 0);
    }

    return drawing;   	
}



/** @function main */
int main(int argv, char* argc[])
{
    //! [mat]
    Mat frame, frame_morph, frame_gray, frame_threshold, frame_label;
    Mat element = getStructuringElement( MORPH_RECT, Size( 14, 14 ), Point(-1,-1) );// Size(6,6) marche mais on a un probleme avec le glare
    Mat img, img_edge, labels, centroids, img_color, stats;
    
    //! [cap]
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,800);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);	//768
    //VideoWriter video1("source.avi",CV_FOURCC('M','J','P','G'),10, Size(800,600));
    //VideoWriter video("centroids.avi",CV_FOURCC('M','J','P','G'),2, Size(800,600));
     
    //! [window]
    //namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Centroid Detection", WINDOW_AUTOSIZE);
    namedWindow("Object Detection", WINDOW_AUTOSIZE);
	struct timespec start, end;	// initiate start and ends values of clock
	clock_gettime(CLOCK_REALTIME, &start); //starts clock
	uint32_t nbLoop=0;
    while((char)waitKey(1)!='q'){
        
        cap>>frame;
        if(frame.empty()) break;
        
        //-- Detect the object based on RGB Range Values
        cvtColor( frame, frame_gray, COLOR_BGR2GRAY );	//Grey scale convertion
        threshold(frame_gray, frame_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);  //Otsu thresholding
        morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
		frame_label = find_moments( frame_morph);	// Contours/Centroid      
        

       if(nccomps != nbGraine){	//check if a theres a change in # of seeds in the frame and tell how many there is
		    cout << "Voici le nombre de graine : " << nccomps-1 << endl;	//remove background, returns number of seed detected
			nbGraine=nccomps;
        }
        
        //-- [show]
		//imshow("Video Capture",frame);
		imshow("Centroid Detection",frame_label);
        //imshow("Object Detection",frame_morph);        
        //video.write(frame);
        //video1.write(frame_label);
        nbLoop++;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double difference = (end.tv_sec -start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
    cout << "Ca prend " << difference << "secondes pour faire " << nbLoop << " capture dimage et traitement" << endl;
    cout << "Moyenne de : " << nbLoop/difference << " capture&traitement/secondes " << endl; 
    imwrite("otsu_thresholding.jpg",frame_threshold);
    imwrite("sorted_seeds.jpg",frame_label); 
    imwrite("noise_filtered.jpg",frame_morph); 
    imwrite("gray_scaled.jpg",frame_gray);
    imwrite("original_frame.jpg",frame);  
    //video.release();
    //video1.release();
    cap.release();
    return 0;
}

