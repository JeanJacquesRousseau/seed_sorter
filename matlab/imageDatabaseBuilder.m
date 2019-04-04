close all
clear all
clc
%% Code Matlab pour construire base de donnée d'images
% Nombre d'images voulues
nbImageTotal = 10;

% Interval de temps entre chaque photo.
nbSecondeInterval = 5;

% Type de grain pris en photo
% imageName est également le préfixe du nom des images.
imageName = 'SOY';

% Nom du fichier où sera sauvegarder les photos
directoryName = 'SOY';

% Initialise objet camera et sa resolution
clear('cam')
cam = webcam;
cam.Resolution = '640x480';



%%
% Rajoute extention pour les nom de fichiers des images 
imageName = strcat(imageName,'_%d.jpg');

% Vérifie si le répertoire existe déja, on le créer sinon.
CurrentWorkingDirectory = pwd;
CurrentWorkingDirectory = strcat(CurrentWorkingDirectory,'\',directoryName);
if ~exist(CurrentWorkingDirectory, 'dir')
  mkdir(directoryName);
end

% Chargement des Parametres pour corriger l'effet FishEye de la caméra
load('cameraParams')

% Compteur du nombre image courant
nbImageCourant=0;

% Flag qui indique si la photo a été prise à l'intérieur de la derniere
% seconde
ImageWasTaken =0;

% Démarrage du timer pour pouvoir prendre photos à chaque nbSecondeInterval
tic;

% Incrémente nbImageTotal de un car la premiere photo est a rejeter car
% elle n'est pas suffisament saturée. 
nbImageTotal = nbImageTotal +1; 

% A FAIRE : Actionner moteur du convoyeur. 
    
while nbImageCourant<nbImageTotal

    if mod(floor(toc),nbSecondeInterval) == 0 

        if ImageWasTaken == 0
            image = snapshot(cam);
            image = imresize(undistortFisheyeImage(snapshot(cam), ...
            cameraParams.Intrinsics), [480, 640]);
            string = sprintf(imageName,nbImageCourant);
            string = strcat(CurrentWorkingDirectory,'\', string);
            
            % On rejete la premiere image car elle n'est pas suffisament
            % satuée 
            if(nbImageCourant ~= 0)
                imwrite(image, string);
            end
            
            nbImageCourant = nbImageCourant + 1;
            ImageWasTaken = 1;
        end
    else 
        ImageWasTaken =0;
    end

end


