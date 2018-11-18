/*
Author : David Patry 
*/
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "seed.h"
#include <wiringPi.h>
#include <stdio.h>
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
//vector<Seed> seeds();

vector<Point2f> center;
vector<Point3f> seeds;


vector<Point3f> seedIdentifier(Mat gray)	/// Find centroid
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
	
		/// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 ); 
    
    /// Find contours
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); 
	
	
	vector<cv::Vec3b> colors(nccomps);	/// Define vector of Vec3b # of blob size
	vector<Point3f> graines(nccomps-1);
	
	colors[0] = cv::Vec3b(0,0,0);	/// background pixels remain black.
	
	for( i = 1; i < nccomps; i++ ) {	///Assign color in function of Area size of blob
	
		if( stats.at<int>(i, cv::CC_STAT_AREA) < 1250 ) {
			colors[i] = cv::Vec3b(0,0,255);	///Soybeans in red
			graines[i-1] = Point3f(0,0,1);
		}
		else {
			colors[i] = cv::Vec3b(0,255,0);	///Corn in green
			graines[i-1] = Point3f(0,0,2);
		}
	}
	
    /// Get the moments
    vector<Moments> mu(contours.size() );
    //Set number of seed here
    
    for(uint32_t i = 0; i < contours.size(); i++ ){
		 mu[i] = moments(contours[i], false);
	}
 
    ///  Get the centroid of figures.
    vector<Point2f> mc(contours.size());
    
    //Seed seed;
    for(uint32_t i = 0; i < contours.size(); i++ ){	//
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	
	}	
	vector<Point2f> transfer(contours.size());
	for(uint32_t j = contours.size(); j > 0; j--)	//flips mcs tails and head ist into transfer
	{
		transfer[contours.size()-j]=mc[j];
	}
	
	
	for(uint32_t k = 0; k < nccomps-1; k++ )	//transfer list has repeating values and value 0 is corrupted.
	{											//So I extract non-trivial list into graines
				graines[k].x = transfer[2*k+1].x;
				graines[k].y = transfer[2*k+1].y;
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
	Rect r = Rect(0,200, 250,200);
	rectangle(drawing,r,Scalar(200,150,25),1,8,0);
    //return drawing;   	
    namedWindow( "Debug Window", WINDOW_AUTOSIZE );
    imshow( "Debug Window", drawing);
    //return transfer;
    return graines;
}


void sort(vector<Point3f> input)
{
	
	for (uint32_t i = 0; i <(int)input.size();i++)
	{
		if( (input[i].x >=0 && input[i].x <= 250) && (input[i].y >= 200 && input[i].y <= 400) ){
		
			cout << " ya une graine ";
			char* couleur;
			if(input[i].z ==2) couleur = "verte ";
			if(input[i].z ==1 ) couleur = "rouge ";
			
			
		
			cout << couleur << "dans slot" << endl;
			
			
		}
	}

	
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
    cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);	//768
     
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
		
		seeds=seedIdentifier(frame_morph);	// Contours/Centroid      
        sort(seeds);

       if(nccomps != nbGraine){	//check if a theres a change in # of seeds in the frame and tell how many there is
		    cout << "Voici le nombre de graine : " << nccomps-1 << endl;	//remove background, returns number of seed detected
			nbGraine=nccomps;
        }
        if ((char)waitKey(1)=='r'){ cout << "Voici ou elle sont :" <<endl << seeds << endl;
        cout << "Pis combien delements dans mon tableau" << (int)seeds.size() << endl;
		}
		
		
		
		
		
        //-- [show]
		imshow("Video Capture",frame);
		//imshow("Centroid Detection",frame_label);
        //imshow("Object Detection",frame_morph);        
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
    cap.release();
    return 0;
}

