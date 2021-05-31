clear;close all;clc;

% 原图
img=imread('mars.tif');
figure(1);subplot(131);imshow(img);title('原图');

% 直方图均衡处理
img_eq = histeq(img);
figure(1);subplot(132);imshow(img_eq);title('直方图均衡');

%% 直方图匹配

% 生成目标直方图: d_h
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

d_hc = d_h./sum(d_h(:));%归一化直方图
d_hca=cumsum(d_hc);% 累计归一化直方图
figure;
subplot(121);plot(d_hc);title('目标归一化直方图');
subplot(122);plot(d_hca);title('目标累积归一化直方图');

% 直方图匹配
img_fix = histeq(img,d_hc);
figure(1);subplot(133);imshow(img_fix);title('直方图匹配');

% 匹配后图像的各种直方图
f_h = imhist(img_fix);
f_hc = cumsum(f_h);
f_hca=f_hc/numel(img_fix);

% 原图像的各种直方图
s_h=imhist(img);% 直方图
s_hc=cumsum(s_h);% 累积直方图
s_hca=s_hc/numel(img);%累积归一化直方图

% 累计归一化直方图对比
figure;hold on; 
plot(0:1/255:1,s_hca,'r.-');
plot(0:1/255:1,d_hca,'b.-');
plot(0:1/255:1,f_hca,'g.-');
plot(0:1/255:1,0:1/255:1,'k--','LineWidth',1);
legend('原图累计直方图','理想目标累计直方图','匹配后真实累计直方图');
