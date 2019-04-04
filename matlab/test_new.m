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
%%

while true
image = snapshot(cam);

h_image = rgb2hsv(image);
s_threshold = graythresh(h_image(:,:,2));
% h_threshold = graythresh(h_image(:,:,1));
se = strel('disk',2);
s_bw = imbinarize(medfilt2(h_image(:,:,2)), s_threshold*1);
% h_bw = ~imbinarize(medfilt2(h_image(:,:,1)), h_threshold*1);

test = imclose(imopen(s_bw, se), se);

% imshow(test)


% mask_r = image(:,:,1) > 200;
% 
% imshow(test & mask_r)
        imshowpair(test, image,'montage')
% imshow(test)
% figure; imshow(image);

end

