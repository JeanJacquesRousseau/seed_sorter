
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(){
	Mat image = imread("exemple.jpg",1);
	namedWindow("image", CV_WINDOW_FREERATIO);
	imshow("image",image);
	calcHist("exemple.jpg");
	waitKey(0);
	
	//Output Image BGR
	Mat	OutputImage;
	
	//Blue - Green - Red 
	inRange(image,Scalar(128,185,207),Scalar(91,142,182), OutputImage);
	
	namedWindow("output sorting", CV_WINDOW_FREERATIO);
	imshow("output sorting", OutputImage);
	waitKey(0);
	//255. 194, 102 - 255, 153, 0

}
