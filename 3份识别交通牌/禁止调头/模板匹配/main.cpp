#include<opencv2\opencv.hpp>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2/video/video.hpp"
#include<fstream>
#include <cstdlib>
#include<iostream>

using namespace std;
using namespace cv;
typedef unsigned char BYTE;
Mat TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src);
int main()
{
	Mat src = imread("../picture/src.jpg", 1);
	Mat Template = imread("../picture/temp.png", 1);
	Mat pt = src;
	Mat dst;
	pt.data = new BYTE[src.cols*src.rows];
	memset(pt.data, 255, src.cols*src.rows);
	dst = TemplateMatch(&pt, &Template, &src);
	
	cout << "禁止调头" << endl;

	namedWindow("原图", 1);//创建窗口
	imshow("原图", src);
	namedWindow("识别结果图", 1);//创建窗口
	imshow("识别结果图", dst);
	//namedWindow("保存", 1);//创建窗口
	//imshow("保存", pt);
	waitKey(0);
	return 0;
}

Mat TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src)
{

	//循环变量
	int i, j, m, n;
	ofstream out("R.txt");
	double dSumT; //模板元素的平方和
	double dSumS; //图像子区域元素的平方和
	double dSumST; //图像子区域和模板的点积	

	//响应值
	double R;

	//记录当前的最大响应
	double MaxR;

	//最大响应出现位置
	int nMaxX=0;
	int nMaxY=0;

	int nHeight = src->rows;
	int nWidth = src->cols;
	//模板的高、宽
	int nTplHeight = pTemplate->rows;
	int nTplWidth = pTemplate->cols;

	//计算 dSumT
	dSumT = 0;
	for (m = 0; m < nTplHeight; m++)
	{
		for (n = 0; n < nTplWidth; n++)
		{
			// 模板图像第m行，第n个象素的灰度值
			int nGray = *pTemplate->ptr(m, n);

			dSumT += (double)nGray*nGray;
		}
	}

	//找到图像中最大响应的出现位置
	MaxR = 0;
	for (i = 0; i < nHeight - nTplHeight + 1; i++)
	{
		out << endl;

		for (j = 0; j < nWidth - nTplWidth + 1; j++)
		{
			dSumST = 0;
			dSumS = 0;

			for (m = 0; m < nTplHeight; m++)
			{
				for (n = 0; n < nTplWidth; n++)
				{
					// 原图像第i+m行，第j+n列象素的灰度值
					int nGraySrc = *src->ptr(i + m, j + n);

					// 模板图像第m行，第n个象素的灰度值
					int nGrayTpl = *pTemplate->ptr(m, n);

					dSumS += (double)nGraySrc*nGraySrc;
					dSumST += (double)nGraySrc*nGrayTpl;
				}
			}

			R = dSumST / (sqrt(dSumS)*sqrt(dSumT));//计算相关响应
			out << R <<' ';

			//与最大相似性比较
			if (R > MaxR)
			{
				MaxR = R;
				nMaxX = j;
				nMaxY = i;
			}
		}
	}

	Mat dst;
	src->copyTo(dst);
	
	rectangle(dst, cvPoint(nMaxX, nMaxY ), cvPoint(nMaxX + pTemplate->cols , nMaxY + pTemplate->rows ), Scalar(255, 0, 0), 2, 1, 0);
	
	imwrite("result.bmp", dst);

	return dst;
}