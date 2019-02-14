/*
Author : David Patry

Compile command :
"g++ -Wall -o "main" "main.cpp" joystick.cc -std=c++0x -lwiringPi -lpthread $(pkg-config --libs --cflags opencv) $(pkg-config --libs --cflags harfbuzz)"
Run command : "./main"

*/
#include "seed.cpp"
#include "joystick.hh"
#include <unistd.h>
#include <string.h>

//Faire cd ./servod dans "/home/pi/servoblaster/PiBits/ServoBlaster/user"
//"echo P1-12=[60,160] > /dev/servoblaster"
/** Global Variables */
uint8_t choix;
int thresh = 140;
int max_thresh = 255;
uint32_t nbGraine=0;
uint32_t nccomps=0;
bool modeAuto = true;

//Motor configs
#define motorPin	15
#define motorD1		14
#define motorD2		18
uint32_t PWM = 0;
int servoCommand =0;
int servoInput = 130;
int motorUpdate = 200;

//PS3 Controller
Joystick joystick("/dev/input/js0");

//Screen Resolution
#define HEIGHT	600//768
#define WIDTH	800//1024

//Vector of seeds containing x,y positions and seed type
vector<Point3f> seeds;


//Servo
// pwmFrequency (Hz) = 19.2e6 Hz / pwnClock / pwmRange.
// pwmSetClock()
// pwmSetRange()



long maper(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void keyboardControl(uint8_t choix ){


    switch(choix){

	case 'r':
	    cout << "Voici ou elle sont :" <<endl << seeds << endl;
	    system("echo P1-12=160 > /dev/servoblaster");
	    cout << "Pis combien delements dans mon tableau : " << (int)seeds.size() << endl;
	    //cout << "PWM : " << PWM << endl;
	    break;

	case 'f':
	    if(PWM <= 95){
	    //PWM = PWM+5;
	    system("echo P1-12=110 > /dev/servoblaster");
	    //softPwmWrite(motorPin,PWM);
	    //cout << "Duty cycle : "<<PWM<<"%" <<endl;
	    }
	    else cout << "Tu as atteint la vitesse maximal pour le convoyeur" << endl;
	    break;

	case 's':
	    if(PWM >= 5){
	    //PWM = PWM-5;
	    //softPwmWrite(motorPin,PWM);
	    //cout << "Duty cycle : "<<PWM<<"%" <<endl;
	    }
	    else cout << " Il n'y a pas de reculons pour le convoyeur" << endl;
	case 32:
	    modeAuto=!modeAuto;
	    if(modeAuto == 0){
		cout << "Mode Manuel activee" << endl;
		//cap.release();
		if (!joystick.isFound()){
		    printf("open failed.\n");
		    exit(1);
		}

	    }
	    else{
		cout << "Mode Automatique activee" << endl;
		//VideoCapture cap(0);
		//cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);		//1024
		//cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);	//768
	    }

	break;

	default: break;
}

}
int main(int argv, char* argc[]){
    //! [mat]
    Mat frame, frame_original, frame_morph, frame_gray, frame_threshold, frame_label;
    Mat element = getStructuringElement( MORPH_RECT, Size( 11, 11 ), Point(-1,-1) );// Size(6,6) marche mais on a un probleme avec le glare

    Seed seed;
    Mat watershed;
    //Mat eclatee = imread(argc[1]);

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
    //namedWindow("Video Capture", WINDOW_AUTOSIZE);
    namedWindow("Keyboard Control", WINDOW_AUTOSIZE);
   // namedWindow("Object Detection", WINDOW_AUTOSIZE);


    //! [performance measurement]
	struct timespec start, end;	// initiate start and ends values of clock
	clock_gettime(CLOCK_REALTIME, &start); //starts clock
	uint32_t nbLoop=0;

    while((char)choix != 27){	// ~26 frames/secs with no algorithm and no show

            cap>>frame;
	    frame.copyTo(frame_original);
            if(frame.empty())
		{
		    cout << " sa marche pas" << endl;
	            break;
	        }
	    //imshow("Keyboard Control",frame);

          //! Background extraction
	    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );	//Grey scale convertion
	    threshold(frame_gray, frame_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);  //Otsu thresholding
	    morphologyEx(frame_threshold, frame_morph, MORPH_OPEN , element);	// Noise filter
	    //frame.copyTo(watershed, frame_morph);
	    //watershed = bitwise_and(frame,frame,frame_morph);
	    //imshow("mask", watershed);
	    seeds=seed.identifier(frame_morph);	// Input a masked image where backgrounds has been extracted and output vector of floats with element (centroid_x, centroid_y, type)
	    //seed.sort(seeds);  // Takes in vector of seeds and do motor control to sort
	    frame_label = seed.draw(seeds, frame_morph,frame);	// TODO : Issue with drawing random centroids points on side of screen
	    //seed.printCount(seeds);	// TODO : work to be done here.
	
	    if(nbGraine != seeds.size()){	//check if a theres a change in # of seeds in the frame and tell how many there is
		    //cout << "Voici le nombre de graine : " << seeds.size() << endl;	//remove background, returns number of seed detected
		    //nbGraine= seeds.size();
	    }



	         //imshow("Centroid Detection",frame_label);
	         nbLoop++;	/// Loop counter


	if(nbLoop%15 == 0) {
	    //Vec3b pixel= ;
	    //cout << (int)frame_original.at<Vec3b>(4,4).val[0] << endl;
	}

        //! [show]
	//imshow("yo el gros", frame_morph);
	//imshow("Video Capture",frame_original);
	//frame_original.copyTo(frame,frame_morph);
	//Mat mask;
	//frame_morph.convertTo(mask,CV_8UC1);
	frame_original.copyTo(frame_morph,frame_morph);
	imshow("mask",frame_morph);
	imshow("og",frame_original);

        //cout << "mode Auto : " << modeAuto << endl;
	choix = waitKey(1);
	keyboardControl(choix);
	//cout << "voici ton choix" << choix << endl;


    }

    //! [performance measurements]
    clock_gettime(CLOCK_REALTIME, &end);
    double difference = (end.tv_sec -start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
    cout << "Ca prend " << difference << "secondes pour faire " << nbLoop << " capture dimage et traitement" << endl;
    cout << "Moyenne de : " << nbLoop/difference << " capture&traitement/secondes " << endl;

    //! [Write Results]
    //imwrite("/home/pi/seed_sorter/image/otsu_thresholding.jpg",frame_threshold);
    //imwrite("/home/pi/seed_sorter/image/identified_seeds.jpg",frame_label);
    //imwrite("/home/pi/seed_sorter/test/watershedEXP2.jpg",watershed);
    //imwrite("/home/pi/seed_sorter/image/grain_avec_tube.jpg",frame_original);
    imwrite("/home/pi/seed_sorter/image/CornWithMask.jpg",frame_morph);


    cap.release();
    return 0;
}
