clear;close all;clc;

% ԭͼ
img=imread('mars.tif');
figure(1);subplot(131);imshow(img);title('ԭͼ');

% ֱ��ͼ���⴦��
img_eq = histeq(img);
figure(1);subplot(132);imshow(img_eq);title('ֱ��ͼ����');

%% ֱ��ͼƥ��

% ����Ŀ��ֱ��ͼ: d_h
u1=0.15;   sig1=0.05;
u2=0.75;    sig2=0.05;
A1=1;   A2=0.07;    
k=0.002;
c1 = A1 *(1/((2*pi)^0.5)*sig1);
c2 = A2 *(1/((2*pi)^0.5)*sig2);
k1 = 2 * (sig1^2);
k2 = 2 * (sig2^2);
z = linspace(0,1,256);
d_h = k + c1*exp(-((z-u1).^2)./k1)+c2 * exp(-((z-u2).^2)./k2);

d_hc = d_h./sum(d_h(:));%��һ��ֱ��ͼ
d_hca=cumsum(d_hc);% �ۼƹ�һ��ֱ��ͼ
figure;
subplot(121);plot(d_hc);title('Ŀ���һ��ֱ��ͼ');
subplot(122);plot(d_hca);title('Ŀ���ۻ���һ��ֱ��ͼ');

% ֱ��ͼƥ��
img_fix = histeq(img,d_hc);
figure(1);subplot(133);imshow(img_fix);title('ֱ��ͼƥ��');

% ƥ���ͼ��ĸ���ֱ��ͼ
f_h = imhist(img_fix);
f_hc = cumsum(f_h);
f_hca=f_hc/numel(img_fix);

% ԭͼ��ĸ���ֱ��ͼ
s_h=imhist(img);% ֱ��ͼ
s_hc=cumsum(s_h);% �ۻ�ֱ��ͼ
s_hca=s_hc/numel(img);%�ۻ���һ��ֱ��ͼ

% �ۼƹ�һ��ֱ��ͼ�Ա�
figure;hold on; 
plot(0:1/255:1,s_hca,'r.-');
plot(0:1/255:1,d_hca,'b.-');
plot(0:1/255:1,f_hca,'g.-');
plot(0:1/255:1,0:1/255:1,'k--','LineWidth',1);
legend('ԭͼ�ۼ�ֱ��ͼ','����Ŀ���ۼ�ֱ��ͼ','ƥ�����ʵ�ۼ�ֱ��ͼ');
