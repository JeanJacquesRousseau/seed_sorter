/*
Author : David Patry 
*/


#include "seed.cpp"

/** Global Variables */
int thresh = 140;
int max_thresh = 255;
uint32_t nbGraine=0;
uint32_t nccomps=0;

//Motor configs
#define motorPin	15
#define motorD1		14
#define motorD2		18
uint32_t PWM = 0;

//Screen Resolution
#define HEIGHT	660//768//600//768
#define WIDTH	880//1024//800//1024

vector<Point3f> seeds;


//Servo
// pwmFrequency (Hz) = 19.2e6 Hz / pwnClock / pwmRange.
// pwmSetClock()
// pwmSetRange()


void keyboardControl(){
		
	if ((char)waitKey(1)=='r'){ 
		cout << "Voici ou elle sont :" <<endl << seeds << endl;
		system("echo 0=60 > /dev/servoblaster");
		cout << "Pis combien delements dans mon tableau : " << (int)seeds.size() << endl;
		//cout << "PWM : " << PWM << endl;
	}
	
	if ((char)waitKey(1) =='f') {
		if(PWM <= 95){
			//PWM = PWM+5;
			system("echo 0=110 > /dev/servoblaster");
			//softPwmWrite(motorPin,PWM);	
			//cout << "Duty cycle : "<<PWM<<"%" <<endl;
		}
		else cout << "Tu as atteint la vitesse maximal pour le convoyeur" << endl;
	}
	
	if ((char)waitKey(1) =='s'){
		if(PWM >= 5){
			//PWM = PWM-5;
			//softPwmWrite(motorPin,PWM);				
			//cout << "Duty cycle : "<<PWM<<"%" <<endl;
		}
		else cout << " Il n'y a pas de reculons pour le convoyeur" << endl;
	}

}

int main(int argv, char* argc[])
{
    //! [mat]
    Mat frame, frame_original, frame_morph, frame_gray, frame_threshold, frame_label;
    Mat element = getStructuringElement( MORPH_RECT, Size( 15, 15 ), Point(-1,-1) );// Size(6,6) marche mais on a un probleme avec le glare

    Seed seed;
    /*
    seed.setCenter(2,4);
    seed.setType(MAIS);
    cout << "Le centre de seed est : " << seed.getPosX() << "," << seed.getPosY() << endl;
    cout << "Le type de seed est : " << seed.getType() << endl;
    */
    //! [motors]
    //wiringPiSetup();
	//pinMode(motorPin, PWM_OUTPUT);
	//pinMode(motorD1, OUTPUT);
	//pinMode(motorD2, OUTPUT);
	//pwmSetClock(2);
	//pwmSetRange(10);
	//pwmWrite(motorPin, PWM);
	//softPwmCreate(motorPin,0,100);
	//softPwmWrite(motorPin,PWM);
	//softServoSetup(0,1,2,3,4,5,6,7);
	//softServoWrite(0,500);
	
    //! [cap]
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);	//768
     
    //! [window]
    namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Keyboard Control", WINDOW_AUTOSIZE);
   // namedWindow("Object Detection", WINDOW_AUTOSIZE);
    
    //! [performance measurement]
	struct timespec start, end;	// initiate start and ends values of clock
	clock_gettime(CLOCK_REALTIME, &start); //starts clock
	uint32_t nbLoop=0;
	
    while((char)waitKey(1)!=27){	// ~26 frames/secs with no algorithm and no show

        cap>>frame;         
        if(frame.empty()) break;
	//imshow("Centroid Detection",frame);
	frame_original=frame;
          

		//! Background extraction
        cvtColor( frame, frame_gray, COLOR_BGR2GRAY );	//Grey scale convertion
        threshold(frame_gray, frame_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);  //Otsu thresholding
        morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
        
	seeds=seed.identifier(frame_morph);	// Input a masked image where backgrounds has been extracted and output vector of floats with element (centroid_x, centroid_y, type)
	//seed.sort(seeds);  // Takes in vector of seeds and do motor control to sort
	frame_label = seed.draw(seeds, frame_morph,frame);	// TODO : Issue with drawing random centroids points on side of screen
	//seed.printCount(seeds);	// TODO : work to be done here.
	/*	
	if(nbGraine != seeds.size()){	//check if a theres a change in # of seeds in the frame and tell how many there is
		cout << "Voici le nombre de graine : " << seeds.size() << endl;	//remove background, returns number of seed detected
		nbGraine= seeds.size();
	}
	*/
		
	keyboardControl();
	imshow("Centroid Detection",frame_label);
		
        //! [show]
	imshow("Video Capture",frame_morph);
	  
        nbLoop++;	/// Loop counter
    }
    
    //! [performance measurements] 
    clock_gettime(CLOCK_REALTIME, &end);
    double difference = (end.tv_sec -start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
    cout << "Ca prend " << difference << "secondes pour faire " << nbLoop << " capture dimage et traitement" << endl;
    cout << "Moyenne de : " << nbLoop/difference << " capture&traitement/secondes " << endl; 
    
    //! [Write Results]
    imwrite("/home/pi/seed_sorter/image/otsu_thresholding.jpg",frame_threshold);
    imwrite("/home/pi/seed_sorter/image/sorted_seeds.jpg",frame_label); 
    imwrite("/home/pi/seed_sorter/image/noise_filtered.jpg",frame_morph); 
    imwrite("/home/pi/seed_sorter/image/gray_scaled.jpg",frame_gray);
    imwrite("/home/pi/seed_sorter/image/original_frame.jpg",frame_original);  
    
    //cap.release();
    return 0;
}
