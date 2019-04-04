close all
clear all
clc
%%
sideLength = 60;
% Initialize objet camera et resolution
clear('cam')
cam = webcam;
% Anciennement 
cam.Resolution = '640x480';

% Chargement du modele entrainé
load('svmQuadModelFull')
load('cameraParams')


while true
    
%     image_orig = imresize(undistortFisheyeImage(snapshot(cam), ...
%         cameraParams.Intrinsics), [480, 640]);
%     image = image_orig;

    image = snapshot(cam);

    
    % Inscription de rectangle noir
    image = insertShape(image,'FilledRectangle',[0 0 640 50], 'Color', 'black', 'Opacity',1);
    image = insertShape(image,'FilledRectangle',[0 380 640 100], 'Color', 'black', 'Opacity',1);
    
    [~, resultIm, iBW, seedVector] = findAndClassify(image, sideLength, svmQuadModelFull);
   
        
    % seedVector : 
    % Vecteur avec la position du grain et son identifiant:
    % (X,Y,type) 
    % type = 0 pour Mais, type = 1 pour Soya 
    
    if ~isempty(resultIm)
        nBgraine = size(seedVector,1);
        for i=1:nBgraine
            i;
            seedVector(i,1:2);
        end
            
         imshow(resultIm);
%          imshow(iBW);

%         imshowpair(resultIm, iBW,'montage')

    end    
end
