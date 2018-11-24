/*
 * Seed.h
 * Created : 2018-17-11 23:03:22
 * Author: David Patry
 */
 
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;
using namespace cv;

#ifndef __SEED_H__
#define __SEED_H__

enum Graine { NOT_IDENTIFIED, MAIS, SOYA }; 		

class Seed
{
	public:
		Seed();
		~Seed();
		void setCenter(float x, float y);
		void setType(Graine sorte);
		float getPosX();
		float getPosY();
		Graine getType();
		
		vector<Point3f> identifier(Mat mask);
		Mat draw(vector<Point3f> seeds , Mat mask, Mat frame);
		void sort(vector<Point3f> seeds);
		void printCount(vector<Point3f> seeds);
		
		
	protected:
	private:
		float center_x;
		float center_y;
		Graine sorte_;
		uint32_t nbGraine_;
		Mat labels_;
		Mat centroids_;
		Mat stats_;
		//Point2f point;	
};



#endif


