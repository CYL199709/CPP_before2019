clear;close all;clc;

Img = imread('aerial.tif');
figure;imshow(Img);title('ԭͼ');
gImg = mat2gray(Img);

myxaxis = 0:255;
hist_gImg = imhist(gImg );
figure;bar(myxaxis,hist_gImg);title('ԭͼ��ֱ��ͼ');

mImg = mat2gray(gImg-0.4,[0 1]);
figure;imshow(mImg);title('�Ҷ�ƽ�ƺ�');
hist_mImg = imhist(mImg );
figure;bar(myxaxis,hist_mImg);title('�Ҷ�ƽ�ƺ��ֱ��ͼ');

sImg = mat2gray(mImg);
hist_sImg = imhist(sImg );
figure;bar(myxaxis,hist_sImg);title('�Ҷ�������ֱ��ͼ');
figure;imshow(sImg);title('�Ҷ������');

close all;
figure;
subplot(121);imshow(gImg);title('ԭͼ');
subplot(122);imshow(sImg);title('�Ҷ�ƽ�������');


figure;bar(myxaxis,hist_gImg);title('ԭͼ��ֱ��ͼ');
Gamma = 4;
gaImg = gImg.^Gamma;
hist_gaImg = imhist(gaImg);
figure;bar(myxaxis,hist_gaImg);title('gamma�任��');
figure;
subplot(121);imshow(gImg);title('ԭͼ');
subplot(122);imshow(gaImg);title('gamma�任��');

eqImg = histeq(gImg);
hist_eqImg = imhist(eqImg);
figure;bar(myxaxis,hist_eqImg);title('ֱ��ͼ�����');
figure;
subplot(121);imshow(gImg);title('ԭͼ');
subplot(122);imshow(eqImg);title('ֱ��ͼ�����');


