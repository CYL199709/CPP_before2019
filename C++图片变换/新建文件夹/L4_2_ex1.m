clear;close all;clc;

Img = imread('aerial.tif');
figure;imshow(Img);title('原图');
gImg = mat2gray(Img);

myxaxis = 0:255;
hist_gImg = imhist(gImg );
figure;bar(myxaxis,hist_gImg);title('原图的直方图');

mImg = mat2gray(gImg-0.4,[0 1]);
figure;imshow(mImg);title('灰度平移后');
hist_mImg = imhist(mImg );
figure;bar(myxaxis,hist_mImg);title('灰度平移后的直方图');

sImg = mat2gray(mImg);
hist_sImg = imhist(sImg );
figure;bar(myxaxis,hist_sImg);title('灰度拉伸后的直方图');
figure;imshow(sImg);title('灰度拉伸后');

close all;
figure;
subplot(121);imshow(gImg);title('原图');
subplot(122);imshow(sImg);title('灰度平移拉伸后');


figure;bar(myxaxis,hist_gImg);title('原图的直方图');
Gamma = 4;
gaImg = gImg.^Gamma;
hist_gaImg = imhist(gaImg);
figure;bar(myxaxis,hist_gaImg);title('gamma变换后');
figure;
subplot(121);imshow(gImg);title('原图');
subplot(122);imshow(gaImg);title('gamma变换后');

eqImg = histeq(gImg);
hist_eqImg = imhist(eqImg);
figure;bar(myxaxis,hist_eqImg);title('直方图均衡后');
figure;
subplot(121);imshow(gImg);title('原图');
subplot(122);imshow(eqImg);title('直方图均衡后');


