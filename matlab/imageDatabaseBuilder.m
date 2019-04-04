close all
clear all
clc
%% Code Matlab pour construire base de donn�e d'images
% Nombre d'images voulues
nbImageTotal = 10;

% Interval de temps entre chaque photo.
nbSecondeInterval = 5;

% Type de grain pris en photo
% imageName est �galement le pr�fixe du nom des images.
imageName = 'SOY';

% Nom du fichier o� sera sauvegarder les photos
directoryName = 'SOY';

% Initialise objet camera et sa resolution
clear('cam')
cam = webcam;
cam.Resolution = '640x480';



%%
% Rajoute extention pour les nom de fichiers des images 
imageName = strcat(imageName,'_%d.jpg');

% V�rifie si le r�pertoire existe d�ja, on le cr�er sinon.
CurrentWorkingDirectory = pwd;
CurrentWorkingDirectory = strcat(CurrentWorkingDirectory,'\',directoryName);
if ~exist(CurrentWorkingDirectory, 'dir')
  mkdir(directoryName);
end

% Chargement des Parametres pour corriger l'effet FishEye de la cam�ra
load('cameraParams')

% Compteur du nombre image courant
nbImageCourant=0;

% Flag qui indique si la photo a �t� prise � l'int�rieur de la derniere
% seconde
ImageWasTaken =0;

% D�marrage du timer pour pouvoir prendre photos � chaque nbSecondeInterval
tic;

% Incr�mente nbImageTotal de un car la premiere photo est a rejeter car
% elle n'est pas suffisament satur�e. 
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
            % satu�e 
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


