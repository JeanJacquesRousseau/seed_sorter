






#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
using namespace std;
using namespace cv;

int main()
{	int frame_width = 640, frame_height = 480;
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,frame_width);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,frame_height);
    if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
    }
    Mat frame, edges;
	
    struct timespec start, end;
    clock_gettime( CLOCK_REALTIME, &start );

    int frames=10;
    for(int i=0; i<frames; i++){
    	capture >> frame;
    	if(frame.empty()){
		cout << "Failed to capture an image" << endl;
		return -1;
    	}
    	//cvtColor(frame, edges, CV_BGR2GRAY);
    	//Canny(edges, edges, 0, 30, 3);
    }

    clock_gettime( CLOCK_REALTIME, &end );
    double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
    cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
    cout << "Capturing and processing " << frames/difference << " frames per second " << endl;
    VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),frames/difference, Size(frame_width,frame_height));
   
   
	while(1){
		 
		Mat frame; 
     
		// Capture frame-by-frame 
		capture >> frame;
  
		// If the frame is empty, break immediately
		if (frame.empty())
		break;
     
		// Write the frame into the file 'outcpp.avi'
		video.write(frame);
    
		// Display the resulting frame    
		imshow( "Frame", frame );
  
		// Press  ESC on keyboard to  exit
		char c = (char)waitKey(1);
		if( c == 27 ) 
				break;
	}
      // When everything done, release the video capture and write object
	capture.release();
	video.release();
 
  // Closes all the windows
  destroyAllWindows();
    return 0;
}

/*
// Example 14-3. Drawing labeled connected components

#include <algorithm>
#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;

int main(int argc, char* argv[]) {

  cv::Mat img, img_edge, labels, centroids, img_color, stats;

  // load image or show help if no image was provided
  if( (argc != 2)
  || (img = cv::imread( argv[1], cv::IMREAD_GRAYSCALE )).empty()
  ) {
	cout << "\nERROR: You need 2 parameters, you had " << argc << "\n" << endl;
    cout << "\nExample 14-3: Drawing labeled connected componnents\n"
    << "Call is:\n" <<argv[0] <<" <path/image>\n"
    << "\nExample:\n" << argv[0] << " ../HandIndoorColor.jpg\n" << endl; 
    return -1;
  }

  cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
  cv::imshow("Image after threshold", img_edge);

  int i, nccomps = cv::connectedComponentsWithStats (
	img_edge, 
	labels,
	stats, 
	centroids
  );
  cout << "Total Connected Components Detected: " << nccomps << endl;

  vector<cv::Vec3b> colors(nccomps+1);
  colors[0] = cv::Vec3b(0,0,0); // background pixels remain black.
  for( i = 1; i <= nccomps; i++ ) {
    colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256);
    
    
    //if( stats.at<int>(i-1, cv::CC_STAT_AREA) < 1000 ) colors[i] = cv::Vec3b(0,0,255);	//Soybeans in red
    //else colors[i] = cv::Vec3b(0,255,0);
    
    if( stats.at<int>(i-1, cv::CC_STAT_AREA) < 100 ) colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black too.
    
    
    cout << "surface : " << stats.at<int>(i-1, cv::CC_STAT_AREA)<< "  couleur : " << colors[i-1] << endl;
  }
  img_color = cv::Mat::zeros(img.size(), CV_8UC3);
  for( int y = 0; y < img_color.rows; y++ )
    for( int x = 0; x < img_color.cols; x++ )
    {
      int label = labels.at<int>(y, x);
      CV_Assert(0 <= label && label <= nccomps);
      img_color.at<cv::Vec3b>(y, x) = colors[label];
    }

  cv::imshow("Labeled map", img_color);
  cv::waitKey();
  return 0;
}
*/
