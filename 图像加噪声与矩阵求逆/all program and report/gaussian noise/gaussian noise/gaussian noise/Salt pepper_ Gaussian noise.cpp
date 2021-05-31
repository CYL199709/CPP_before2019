#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <random>

using namespace cv;
using namespace std;

//椒盐噪声
void AddSaltNoise(Mat img, double SNR)
{//SNR是信噪比，信噪比越小,噪声越多,信噪比为1时,图像不含噪声。 
	Mat dst;
	dst.create(img.rows, img.cols, img.type());
	int SP = img.rows * img.cols;
	//int NP = SP * (1 - SNR);
	int NP = SP *  SNR;


	dst = img.clone();

	for (int i = 0; i<NP; i++)
	{
		//产生随机坐标
		int x = (int)(rand() * 1.0 / RAND_MAX *  (img.rows - 1));
		int y = (int)(rand() * 1.0 / RAND_MAX *  (img.cols - 1));

		int r = rand() % 2;
		if (img.type() == CV_8UC1)
		{ // 灰度图像，单通道8位图像
			if (r)
				dst.at<uchar>(x, y) = 0;
			else
				dst.at<uchar>(x, y) = 255;
		}
		else if (img.type() == CV_8UC3) 
		{ // 彩色图像，3通道图像
			if (r)
			{
				dst.at<Vec3b>(x, y)[0] = 0;
				dst.at<Vec3b>(x, y)[1] = 0;
				dst.at<Vec3b>(x, y)[2] = 0;
			}
			else
			{
				dst.at<Vec3b>(x, y)[0] = 255;
				dst.at<Vec3b>(x, y)[1] = 255;
				dst.at<Vec3b>(x, y)[2] = 255;
			}
		}
	}
	imshow("SaltNoise", dst);
}


//高斯噪声
double GenerateGaussianNoise(double E, double D)
{
	//定义一个特别小的值
	const double epsilon = numeric_limits<double>::min();//返回目标数据类型能表示的最逼近1的正数和1的差的绝对值
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flag为假，构造高斯随机变量
	if (!flag)
		return z1 * D + E;
	double u1, u2;
	//构造随机变量

	do
	{
		u1 = rand()*(1.0 / RAND_MAX);
		u2 = rand()*(1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flag为真构造高斯随机变量X
	z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI * u2);
	z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI * u2);
	return z1 * D + E;
}
void AddGaussianNoise(Mat img, double E, double D, int k)
{//k为高斯噪声的系数,系数越大,高斯噪声越强
	Mat dst;
	dst.create(img.rows, img.cols, img.type());
	dst = img.clone();
	double temp_gray;
	double temp_c3_b;
	double temp_c3_g;
	double temp_c3_r;
	for (int x = 0; x<img.rows; x++)
	{
		for (int y = 0; y<img.cols; y++)
		{
			if (img.type() == CV_8UC1)
			{ // 灰度图像，单通道8位图像
				temp_gray = dst.at<uchar>(x, y) + k*GenerateGaussianNoise(E, D);//加噪声
				if (temp_gray>255)
					temp_gray = 255;
				else if (temp_gray<0)
					temp_gray = 0;
				dst.at<uchar>(x, y) = temp_gray;
			}
			else if (img.type() == CV_8UC3)
			{ // 彩色图像，3通道图像

				temp_c3_b = dst.at<Vec3b>(x, y)[0] + k*GenerateGaussianNoise(E, D);//加噪声
				temp_c3_g = dst.at<Vec3b>(x, y)[1] + k*GenerateGaussianNoise(E, D);//加噪声
				temp_c3_r = dst.at<Vec3b>(x, y)[2] + k*GenerateGaussianNoise(E, D);//加噪声

				//判断数值范围//b
				if (temp_c3_b>255)
					temp_c3_b = 255;
				else if (temp_c3_b<0)
					temp_c3_b = 0;
				//g
				if (temp_c3_g>255)
					temp_c3_g = 255;
				else if (temp_c3_g<0)
					temp_c3_g = 0;
				//r
				if (temp_c3_r>255)
					temp_c3_r = 255;
				else if (temp_c3_r<0)
					temp_c3_r = 0;
				
				dst.at<Vec3b>(x, y)[0] = temp_c3_b;
				dst.at<Vec3b>(x, y)[1] = temp_c3_g;
				dst.at<Vec3b>(x, y)[2] = temp_c3_r;
			}
		}
	}
	imshow("GaussianNoise", dst);
}

int main()
{
	// 打开图像
	Mat image = imread("D:/2.jpg");
	//显示原图像
	imshow("src", image);
	// 调用函数以添加椒盐噪声
	AddSaltNoise(image, 0.2);
	// 调用函数以添加高斯噪声
	AddGaussianNoise(image, 0, 0.5, 40);
	
	waitKey(0);
}