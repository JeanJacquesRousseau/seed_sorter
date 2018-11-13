/* boneCVtiming.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */

//Livrable 1 du cours ELE4205
//Fait par David Patry et Francis Foucault
#include<iostream>
#include<fstream>
#include<string>
#include<opencv2/opencv.hpp>
#include<time.h>
using namespace std;
using namespace cv;

#define NB_RESOLUTION 13

uint16_t input;
string nomFichier;

struct Camera  
{
	int resX[NB_RESOLUTION] = {176, 160, 320, 320, 352, 432, 800, 864, 960, 960, 1184, 1280, 1280};
	int resY[NB_RESOLUTION]= {144, 120, 176, 240, 288, 240, 600, 480, 544, 720, 656, 720, 960};
	double fps[NB_RESOLUTION];
};
Camera camera;

void menu(){

	cout << "Veuillez sélectionner le numéro de la résolution voulue : " << endl;
	for(int i =0; i < NB_RESOLUTION; i++){
		cout << "\t" << i+1 << ". " << camera.resX[i] << "X" << camera.resY[i] << endl;
	}

	cin >> input;
	cout << camera.resX[input-1] << "X" <<camera.resY[input-1] << endl;
}

bool cameraDetect()
{
	system("touch cameradetect.txt");	// Créer ou met à jour un fichier nommé cameradetect.txt
	system("lsusb | grep 05a3:9230 >> cameradetect.txt");	//Écrit le output de la commande lsbusb dans le fichier
	string line;	//string pour lire une ligne inscrite dans cameradetect.txt
	string text;	//string pour constuire tout le texte contenu dans cameradetect.txt
	ifstream myfile("cameradetect.txt");	//Déclaration d'un objet input file stream avec le fichier à lire comme argument
	myfile>>text;
	//if(myfile.is_open()){// On vérifie que l'ouverture s'est bien passé
		if(ws(myfile).eof()){//Le fichier est vide car on a pas reconu notre camera
			cout << "Veuillez brancher la bonne camera et repartir l'application" << endl;
			return false;
		}
		cout <<"camera detectee"<<endl;
/*
		while ( getline (myfile,line) ){	//Tant que la fonction getline s'exécute
			text+=line;		// On concaténe chaque ligne dans "text"
		}

		cout << text << '\n';	// Output le text lu*/
//		myfile.close();	// ferme le fichier lu
	//}
	//else cout << "Unable to open file" << endl;
	
	//A faire
	//parcourir "text" jusqua "ID"
	//passer l'espace
	//extraire la chaine de caractere jusqua au prochaine espace
	//comparer la chaine avec "05a3:9230"
	//retourner un booléean si la comparaison est la même ou pas 

	return true;
}

int main(int argc, char* argv[])
{		
	//Faire test bonne camera  (cameraDetect) A faire
	//Choix résolution Fait 
	//Test moloy Fait
	//Film 5 sec en utilisant résultat de moloy Fait
	
	
	if(cameraDetect()==false) return 0; // Si on ne detecte pas la bonne camera, on termine l'application
	
	if (argv[1]== NULL) nomFichier = "capture-liv1"; //Si on lance l'application sans specifier le nom du fichier video sur lequel on enregistre
	else nomFichier = argv[1]; //On utilise le parametre en argument lors du lancement de lexecutable pour ecrire le nom de fichier video

	menu();	//Demande a lutilisateur de choisir une résolution, store son choix dans input, input sera l'indice du tableau resX et resY contenue dans la struct Camera

    VideoCapture capture(0);	//Demarre un objet VideoCapture avec le peripherique 0
    capture.set(CV_CAP_PROP_FRAME_WIDTH,camera.resX[input-1]);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,camera.resY[input-1]);
    if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
    }
    
    Mat frame, edges;
    struct timespec start, end;

    capture >> frame;	//On prend une premiere frame
	if(frame.empty()){
		cout << "Failed to capture an image" << endl;	// On verifie que ca marche
	return -1;
	}
    capture >> frame;	// On prend une deuxieme frame

    int frames=10;
    clock_gettime( CLOCK_REALTIME, &start );	// On commence le timer avant de prendre 10 frames
    
    for(int i=0; i<frames; i++){
			capture >> frame;
			//cvtColor(frame, edges, CV_BGR2GRAY);
			//Canny(edges, edges, 0, 30, 3);
    }

    clock_gettime( CLOCK_REALTIME, &end );	//Termine le timer
    double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
    cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
    cout << "Capturing and processing " << frames/difference << " frames per second " << endl;
    
    
    double frameRate = frames/difference;	// Calcul du frame rate
    double nBframe = frameRate*5.0;		// Calcul du nombre de frame a prendre en fonction du frame rate pour avoir 5 secondes de vidéo
	Size frameSize(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT));	//On lit la resolution utilisé
	VideoWriter video(nomFichier + ".avi",CV_FOURCC('M','J','P','G'),frameRate, frameSize,true); // On déclare un objet pour faire de l'écriture vidéo
   
	if(!video.isOpened()){	// On vérifie que l'objet est ouvert
		cout << " VideoWriter n'a pas ouvert " << endl;
		getchar();
		return -1;
	}

   for(int j=0; j<nBframe; j++){

       capture.read(frame);
       video.write(frame);
    }
      
   cout << "nombre de frame permmetant de faire 5s de video " << nBframe << "voici le frame rate" << frameRate << endl;

   //string commande = "scp ";
   //commande += nomFichier;
   //commande += ".avi 4205_19@192.168.7.1:/users/Cours/ele4205/19";
   //cout << commande;
   //system(commande);
   //system("knipbit1");
   system("rm cameradetect.txt");	// On efface le fichier sur lequel on a ecrit pour pouvoir reutilise l'application
   return 0;
}

