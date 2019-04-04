clear all
clc
close all
%%
sideLength = 60;


%% Begin with soys
list = extractfield(dir('soy'),'name');
list = list(3:end);

soyVecs = [];
for i = 1:length(list)
    I = imread(['soy/',list{i}]);
    soyVecs = [soyVecs; findAndComputeFeatures(I, sideLength)'];
end

% Corn
list = extractfield(dir('corn'),'name');
list = list(3:end);

cornVecs = [];
for i = 1:length(list)
    I = imread(['corn/',list{i}]);

    cornVecs = [cornVecs; findAndComputeFeatures(I, sideLength)'];
end

data = [cornVecs, zeros(size(cornVecs,1), 1); soyVecs, ones(size(soyVecs,1), 1)];

%%
