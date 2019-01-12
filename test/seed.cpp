/*
 * Seed.cpp
 * Created : 2018-17-11 23:03:22
 * Author: David Patry
 */

#include "seed.h"

//default constructor
Seed::Seed() : center_x(0), center_y(0), sorte_(NOT_IDENTIFIED), armed_(false) {}

Seed::~Seed(){}

vector<Point3f> Seed::identifier(Mat mask){
	
	
    
    nbGraine_ = cv::connectedComponentsWithStats (// Find # of blob (seeds)
		mask, 
		labels_,
		stats_, 
		centroids_
	);
	//nbGraine_ = nbGraine_ -1;
	
	vector<Point3f> graines(nbGraine_ -1);	/// Define 3D space vector for x,y, type of seed
	
		
	for(uint32_t i = 1; i < nbGraine_; i++ ) {	///Assign color in function of Area size of blob
		
		graines[i-1].x = (float)centroids_.at<double>(i,0);	// element i=0 of centroids is backgrounds centroids
		graines[i-1].y = (float)centroids_.at<double>(i,1);
	
		if( stats_.at<int>(i, cv::CC_STAT_AREA) < 650 ) {	// element i = 0 area is backgrounds area	AREA = 1250 for 800X600 Res
			//colors[i] = cv::Vec3b(0,0,255);	///Soybeans in red
			graines[i-1].z = 1;
		}
		else if( stats_.at<int>(i, cv::CC_STAT_AREA) > 650 ) { //&& stats_.at<int>(i, cv::CC_STAT_AREA) < 2250
			//colors[i] = cv::Vec3b(0,255,0);	///Corn in green
			graines[i-1].z = 2;
		}
		else {
			//color[i] = cv::Vec3b(0,165,255);
			graines[i-1].z =3;
		}
		
	}
	

    return graines;

}


Mat Seed::draw(vector<Point3f> seeds , Mat mask, Mat frame){

	vector<cv::Vec3b> colors(nbGraine_);	/// Define a color vector of size # of seeds + background	    
	colors[0] = cv::Vec3b(0,0,0);	/// background pixels remain black.

	for(int i =1; i <=(int)seeds.size() ;i++){	/// Assign red to soybean(1) and green to corn(2) 
			
			if(seeds[i-1].z ==1) colors[i] = cv::Vec3b(0,0,255);
			else if(seeds[i-1].z ==2)colors[i]= cv::Vec3b(0,255,0);
			//else if(seeds[i-1].z == 3) colors[i] = cv::Vec3b(0,165,255);	
			
		
	}
	
	
	Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Scalar color = Scalar(167,151,0);	/// Contours colors is teal ish
    
	Canny( mask, canny_output, 50, 150, 3 ); /// Detect edges using canny
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); /// Find contours
    //Mat drawing(canny_output.size(), CV_8UC3, Scalar(255,255,255));	///Set a blank image of same size as cannyoutput to write on
       
	for( int y = 0; y < frame.rows; y++ )
		for( int x = 0; x < frame.cols; x++ ){
			int label = labels_.at<int>(y, x);	
			CV_Assert(0 <= label && label <= nbGraine_);		///Run time check
			//drawing.at<cv::Vec3b>(y, x) = colors[label];	///Write colors to coresponding seed on drawing frame
			if(label != 0) frame.at<cv::Vec3b>(y, x) = colors[label];	///Write colors to coresponding seed on drawing frame
	}
	
	
    for( uint32_t i = 0; i< contours.size(); i++ ){
        drawContours(frame, contours, i, color, 2, 8, hierarchy, 0, Point());	/// Draw contours of seeds in teal 
        circle(frame, Point2f(seeds[i].x,seeds[i].y), 4, Scalar(255,0,0), -1, 7, 0);	///Draw center of seeds in blue
    }
    
    
	Rect target_area = Rect(10, 200, 200,200);
	rectangle(frame,target_area,Scalar(200,150,25),2.3,8,0);		///Draw blue rectangle in target area   	
	//putText(frame, "pute",Point2f(0,0), FONT_HERSHEY_PLAIN,2, Scalar(0,0,255),2,8,false);
    //namedWindow( "Debug Window", WINDOW_AUTOSIZE );
    //imshow( "Debug Window", frame);
    return frame;
}


void Seed::sort(vector<Point3f> seeds){
	
	if(armed_){
		for (uint32_t i = 0; i <nbGraine_ -1;i++)
		{
			if( (seeds[i].x >=10 && seeds[i].x <= 210) && (seeds[i].y >= 200 && seeds[i].y <= 400) ){		
				
				//cout << " ya une graine ";
				//char* couleur;
				//if(seeds[i].z ==1) system("echo 0=60 > /dev/servoblaster");
				if(seeds[i].z ==2 ){
					timer_ = millis();
					system("echo P1-12=120 > /dev/servoblaster");
					cout<< "Tri en cours" << endl;		
					armed_ = false;
					//break;
				}//cout << couleur << "dans slot" << endl;
			}
			//else system("echo 0=60 > /dev/servoblaster");
			
		}
	}
	
	if(millis() - timer_ >2400 && armed_ == false) {
		armed_ =true;
		system("echo P1-12=200 > /dev/servoblaster");
	}
}

void Seed::printCount(vector<Point3f> seeds){
	
	//cout << " nbGraine_ : " << nbGraine_ - 1<< endl;
	//cout << " seeds.size() : " << seeds.size() << endl;
	
	if(nbGraine_ -1 != seeds.size()){	//check if a theres a change in # of seeds in the frame and tell how many there is
	    cout << "Voici le nombre de graine : " << seeds.size() << endl;	//remove background, returns number of seed detected
		//nbGraine_= seeds.size();
	}

}


void Seed::setCenter(float x, float y){
	center_x=x;
	center_y=y;
}

void Seed::setType(Graine sorte) {
	sorte_ = sorte;
}

float Seed::getPosX(){	
	return center_x;
}

float Seed::getPosY(){	
	return center_y;
}

Graine Seed::getType(){
	return sorte_;
}

