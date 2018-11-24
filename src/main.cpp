/*
Author : David Patry 
*/


#include "seed.cpp"



/** Function Headers */
/** Global Variables */
Mat src_gray;
Mat labels;
int thresh = 140;
int max_thresh = 255;
RNG rng(12345);
uint32_t nbGraine=0;
uint32_t nccomps=0;

//Motor configs
#define motorPin	15
#define motorD1		14
#define motorD2		18
uint32_t PWM = 0;

//Screen Resolution
#define HEIGHT	600//768
#define WIDTH	800//1024

vector<Point3f> seeds;



void keyboardControl(){
		
	if ((char)waitKey(1)=='r'){ 
		cout << "Voici ou elle sont :" <<endl << seeds << endl;
		cout << "Pis combien delements dans mon tableau : " << (int)seeds.size() << endl;
		cout << "PWM : " << PWM << endl;
	}
	
	if ((char)waitKey(1) =='f') {
		if(PWM <= 95){
			PWM = PWM+5;
			softPwmWrite(motorPin,PWM);	
			cout << "Duty cycle : "<<PWM<<"%" <<endl;
		}
		else cout << "Tu as atteint la vitesse maximal pour le convoyeur" << endl;
	}
	
	if ((char)waitKey(1) =='s'){
		if(PWM >= 5){
			PWM = PWM-5;
			softPwmWrite(motorPin,PWM);				
			cout << "Duty cycle : "<<PWM<<"%" <<endl;
		}
		else cout << " Il n'y a pas de reculons pour le convoyeur" << endl;
	}

}

int main(int argv, char* argc[])
{
    //! [mat]
    Mat frame, frame_morph, frame_gray, frame_threshold, frame_label;
    Mat element = getStructuringElement( MORPH_RECT, Size( 15, 15 ), Point(-1,-1) );// Size(6,6) marche mais on a un probleme avec le glare

    Seed seed;
    /*
    seed.setCenter(2,4);
    seed.setType(MAIS);
    cout << "Le centre de seed est : " << seed.getPosX() << "," << seed.getPosY() << endl;
    cout << "Le type de seed est : " << seed.getType() << endl;
    */
    //! [motors]
    wiringPiSetupGpio();
	pinMode(motorPin, PWM_OUTPUT);
	pinMode(motorD1, OUTPUT);
	pinMode(motorD2, OUTPUT);
	softPwmCreate(motorPin,0,100);
	softPwmWrite(motorPin,PWM);
	
    //! [cap]
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);	//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);	//768
     
    //! [window]
    //namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Keyboard Control", WINDOW_AUTOSIZE);
    //namedWindow("Object Detection", WINDOW_AUTOSIZE);
    
    //! [performance measurement]
	struct timespec start, end;	// initiate start and ends values of clock
	clock_gettime(CLOCK_REALTIME, &start); //starts clock
	uint32_t nbLoop=0;
	
    while((char)waitKey(1)!=27){	// ~26 frames/secs with no algorithm and no show

        cap>>frame;
        if(frame.empty()) break;
        
        //-- Detect the object based on RGB Range Values
        
        cvtColor( frame, frame_gray, COLOR_BGR2GRAY );	//Grey scale convertion
        threshold(frame_gray, frame_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);  //Otsu thresholding
        morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
        
		seeds=seed.identifier(frame_morph);
		seed.sort(seeds);  			
		//seed.draw(seeds, frame_morph,frame);

		//seed.printCount(seeds);
		if(nbGraine != seeds.size()){	//check if a theres a change in # of seeds in the frame and tell how many there is
			cout << "Voici le nombre de graine : " << seeds.size() << endl;	//remove background, returns number of seed detected
			nbGraine= seeds.size();
		}

		
		keyboardControl();
		
        //-- [show]
		//imshow("Video Capture",frame_morph);
		//imshow("Centroid Detection",frame_label);
        //imshow("Object Detection",frame_threshold);        
        
        nbLoop++;	/// Loop counter
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double difference = (end.tv_sec -start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
    cout << "Ca prend " << difference << "secondes pour faire " << nbLoop << " capture dimage et traitement" << endl;
    cout << "Moyenne de : " << nbLoop/difference << " capture&traitement/secondes " << endl; 
    //imwrite("otsu_thresholding.jpg",frame_threshold);
    //imwrite("sorted_seeds.jpg",frame_label); 
    //imwrite("noise_filtered.jpg",frame_morph); 
    //imwrite("gray_scaled.jpg",frame_gray);
    //imwrite("original_frame.jpg",frame);  
    cap.release();
    return 0;
}
