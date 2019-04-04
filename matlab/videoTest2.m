close all
clear all
clc
%%
sideLength = 60;
vi = VideoReader('seedvideo.avi');
vo = VideoWriter('output_2', 'MPEG-4');
vo.FrameRate = vi.FrameRate;
open(vo);

load('svmQuadModelFull')

while hasFrame(vi)
    image = readFrame(vi);
    [~, resultIm] = findAndClassify(image, sideLength, svmQuadModelFull);
    writeVideo(vo, resultIm);
    
end
close(vo)