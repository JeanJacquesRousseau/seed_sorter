close all
clear all
clc
%%
sideLength = 60;
% Initialize objet camera et resolution
clear('cam')
cam = webcam;
cam.Resolution = '1280x1024';

% Chargement du modele entrainé
load('svmQuadModelFull')

while true
    image = snapshot(cam);
    [~, resultIm, centres] = findAndClassify(image, sideLength, svmQuadModelFull);
    %writeVideo(vo, resultIm);
%     temp = size(centres);
%     nBgraine = temp(1);
%     for i=1:nBgraine
%         i
%         centres(i).Centroid
%     end
    imshow(resultIm);
end
%imwrite(resultIm, 'lastshot.jpg');
% close(vo)