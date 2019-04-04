function [ featureVectors ] = findAndComputeFeatures( inputImage, sideLength )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

%% Init
% defaults
cellSize = [8, 8];
blockSize = [2, 2];
blockOverlap = ceil(blockSize/2);
numBins = 9;


blocksPerImage = floor(([sideLength, sideLength]./cellSize - blockSize)./(blockSize - blockOverlap) + 1);
hogN = prod([blocksPerImage, blockSize, numBins]);

%% Segmentation
threshold = graythresh(rgb2gray(inputImage))*1.2;
iBW = imbinarize(rgb2gray(inputImage),threshold);
iBW = bwareaopen(iBW, 300);

% Debug
% imshow(iBW)

%% Features compute

imCC = bwconncomp(iBW);
labels = labelmatrix(imCC);

bbs = regionprops(imCC, 'BoundingBox');
orientations = regionprops(imCC, 'Orientation');
% centroids = regionprops(imCC, 'Centroid');

featureVectors = zeros(hogN, imCC.NumObjects);

if imCC.NumObjects >= 1
    for i=1:imCC.NumObjects
        bb = bbs(i).BoundingBox;
        x = round(bb(1));
        y = round(bb(2));
        if bb(3) > bb(4)
            padding = floor((bb(3) - bb(4))/2);
            y = max(y - padding, 1);
            bb(4) = bb(4) + 2*padding + floor(bb(3) - bb(4)) - 2*padding;
        elseif bb(4) > bb(3)
            padding = floor((bb(4) - bb(3))/2);
            x = max(x - padding, 1);
            bb(3) = bb(3) + 2*padding + floor(bb(4) - bb(3)) - 2*padding;
        end
        % Better border check and size fit
        subI = zeros(bb(4), bb(4), 3, 'uint8');
        subL = zeros(bb(4), bb(4), 'uint8');
        maxY = min(size(inputImage, 1), y + bb(4));
        maxX = min(size(inputImage, 2), x + bb(3));
        
        
        %
        subI(1:(maxY - y + 1), 1:(maxX - x + 1),:) = inputImage(y:maxY, x:maxX, :);
        subL(1:(maxY - y + 1), 1:(maxX - x + 1)) = labels(y:maxY, x:maxX);
        mask = subL == i;
        mask(:,:,2) = mask(:,:,1);
        mask(:,:,3) = mask(:,:,1);
        
        maskedSubI = subI;
        maskedSubI(~mask) = 0;
        
        maskedSubI = imrotate(maskedSubI, -orientations(i).Orientation);
        s = sideLength/size(maskedSubI,1);
        maskedSubI = imresize(maskedSubI, s);
        
%         % Debug
%         test = rgb2hsv(maskedSubI);
%         [counts, loc] = imhist(test(:,:,1));
%         mode = find(max(counts(3:end)) == counts);
%         modeloc = loc(mode(1));
        
        featureVectors(:, i) = extractHOGFeatures(maskedSubI);
    end
end

end

