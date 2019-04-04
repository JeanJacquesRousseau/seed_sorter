function [ featureVectors ] = computeFeatures( im, imageBW, sideLength)
%COMPUTEHOG Summary of this function goes here
%   Detailed explanation goes here

%% Init
% defaults
cellSize = [8, 8];
blockSize = [2, 2];
blockOverlap = ceil(blockSize/2);
numBins = 9;


blocksPerImage = floor(([sideLength, sideLength]./cellSize - blockSize)./(blockSize - blockOverlap) + 1);
hogN = prod([blocksPerImage, blockSize, numBins]);

%% Compute

imCC = bwconncomp(imageBW);
labels = labelmatrix(imCC);

bbs = regionprops(imCC, 'BoundingBox');
orientations = regionprops(imCC, 'Orientation');

featureVectors = zeros(hogN, imCC.NumObjects);

if imCC.NumObjects >= 1
    for i=1:imCC.NumObjects
        bb = bbs(i).BoundingBox;
        x = round(bb(1));
        y = round(bb(2));
        if bb(3) > bb(4)
            padding = floor((bb(3) - bb(4))/2);
            y = max(y - padding, 1);
            bb(4) = bb(4) + 2*padding;
        elseif bb(4) > bb(3)
            padding = floor((bb(4) - bb(3))/2);
            x = max(x - padding, 1);
            bb(3) = bb(3) + 2*padding;
        end
		
		% Modification pour respecter taille des matrices
		
		pixelY = y+bb(4);
		pixelX = x+bb(3);
		
		if pixelY < 600
			pixelY = 600;
		end
		
		if pixelX < 800
			pixelX = 600;
		end
		
        subI = im(y:(pixelY), x:(pixelX), :);
        subL = labels(y:(pixelY), x:(pixelX));
        mask = subL == i;
        mask(:,:,2) = mask(:,:,1);
        mask(:,:,3) = mask(:,:,1);
        
        maskedSubI = subI;
        maskedSubI(~mask) = 0;
        
        maskedSubI = imrotate(maskedSubI, -orientations(i).Orientation);
        s = sideLength/size(maskedSubI,1);
        maskedSubI = imresize(maskedSubI, s);
        
        
        featureVectors(:, i) = extractHOGFeatures(maskedSubI);
    end
end

end

