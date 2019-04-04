clear all
clc
close all
%% Params
corn = imread('MaisOnly.jpg');
soy = imread('SoyaOnly.jpg');
sideLength = 60;

%% Init
% defaults
cellSize = [8, 8];
blockSize = [2, 2];
blockOverlap = ceil(blockSize/2);
numBins = 9;


blocksPerImage = floor(([sideLength, sideLength]./cellSize - blockSize)./(blockSize - blockOverlap) + 1);
hogN = prod([blocksPerImage, blockSize, numBins]);

%% Corn
cornT = imbinarize(rgb2gray(corn),graythresh(rgb2gray(corn)));
cornT = bwareaopen(cornT, 100);
imshow(cornT)
cornS = corn;

mask = cat(3, cornT, cornT, cornT);
cornS(mask == 0) = 0;
imshow(cornS)

%% Soy
soyT = imbinarize(rgb2gray(soy),graythresh(rgb2gray(soy)));
soyT = bwareaopen(soyT, 100);

% Testing area filtering
% Because soys are touching each others
% TODO: Takes photos without touching soys
soyT = bwareafilt(soyT, [100, 650]);

%
imshow(soyT)
soyS = soy;

mask = cat(3, soyT, soyT, soyT);
soyS(mask == 0) = 0;
imshow(soyS)

% Pad array because soy are too close to border
% TODO: Takes photos with no soy on borders

soyT = padarray(soyT, [10,10]);
soy = padarray(soy, [10,10]);

%% Compute vectors

cornVecs = computeFeatures(corn, cornT, sideLength);
soyVecs = computeFeatures(soy, soyT, sideLength);

%% Prepare data for learning

data = [cornVecs', zeros(size(cornVecs,2), 1); soyVecs', ones(size(soyVecs,2), 1)];

%% Training
trainedModel = trainClassifierTest(data);

%% While loop to take pictures
clear('cam');
cam = webcam(1);
cam.Resolution = '800X600';
load('trainedModelLinearSVM')

image = snapshot(cam)
imwrite(image, 'testwebcam.jpg')
%% 
while true
mixed = snapshot(cam);
mixedT = imbinarize(rgb2gray(mixed),graythresh(rgb2gray(mixed)));
mixedT = bwareaopen(mixedT, 300);
% imshow(mixedT)
% mixedS = mixed;
% 
% mask = cat(3, mixedT, mixedT, mixedT);
% mixedS(mask == 0) = 0;
% imshow(mixedS)

vecs = computeFeatures(mixed, mixedT, sideLength);

imCC = bwconncomp(mixedT);
centroids = regionprops(imCC, 'Centroid');
labels = labelmatrix(imCC);

fit = trainedModel.predictFcn(vecs');

% results = label2rgb(labels);

texts = cell(imCC.NumObjects,1);
positions = zeros(imCC.NumObjects, 2);

for i = 1:imCC.NumObjects
    c = centroids(i).Centroid;
    c = round(c);
    if fit(i) == 0
        texts{i} = 'Corn';
    else
        texts{i} = 'Soy';
    end
    positions(i,:) = c;
end

results = insertText(label2rgb(labels), positions, texts);
imshow(results);
% imwrite(results, 'results.jpg')
end




%% Validate
load('trainedModelLinearSVM')
mixed = imread('testwebcam.jpg');
mixedT = imbinarize(rgb2gray(mixed),graythresh(rgb2gray(mixed)));
mixedT = bwareaopen(mixedT, 300);
% imshow(mixedT)
% mixedS = mixed;
% 
% mask = cat(3, mixedT, mixedT, mixedT);
% mixedS(mask == 0) = 0;
% imshow(mixedS)

vecs = computeFeatures(mixed, mixedT, sideLength);

imCC = bwconncomp(mixedT);
centroids = regionprops(imCC, 'Centroid');
labels = labelmatrix(imCC);

fit = trainedModel.predictFcn(vecs');

% results = label2rgb(labels);

texts = cell(imCC.NumObjects,1);
positions = zeros(imCC.NumObjects, 2);

for i = 1:imCC.NumObjects
    c = centroids(i).Centroid;
    c = round(c);
    if fit(i) == 0
        texts{i} = 'Corn';
    else
        texts{i} = 'Soy';
    end
    positions(i,:) = c;
end

results = insertText(label2rgb(labels), positions, texts);
imshow(results);
imwrite(results, 'results.jpg')


%% Testing with chain
load('trainedModelLinearSVM')
mixed = imread('grain_avec_chaine.jpg');
mixedT = imbinarize(rgb2gray(mixed),graythresh(rgb2gray(mixed)));
mixedT = bwareaopen(mixedT, 300);

mixedHSV = rgb2hsv(mixed);
mask = mixedHSV(:,:,2) < 0.1;
mask(:,:,2) = mask(:,:,1);
mask(:,:,3) = mask(:,:,1);

mixedHSV_masked = mixedHSV;
mixedHSV_masked(mask) = 0;
imshow(mixedHSV_masked)

% vecs = computeFeatures(mixed, mixedT, sideLength);
% 
% imCC = bwconncomp(mixedT);
% centroids = regionprops(imCC, 'Centroid');
% labels = labelmatrix(imCC);
% 
% fit = trainedModel.predictFcn(vecs');
% 
% texts = cell(imCC.NumObjects,1);
% positions = zeros(imCC.NumObjects, 2);
% 
% for i = 1:imCC.NumObjects
%     c = centroids(i).Centroid;
%     c = round(c);
%     if fit(i) == 0
%         texts{i} = 'Corn';
%     else
%         texts{i} = 'Soy';
%     end
%     positions(i,:) = c;
% end
% 
% results = insertText(label2rgb(labels), positions, texts);
% imshow(results);
% imwrite(results, 'results_chain.jpg')