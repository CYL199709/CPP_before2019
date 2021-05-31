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
	
	cout << "��ֹ��ͷ" << endl;

	namedWindow("ԭͼ", 1);//��������
	imshow("ԭͼ", src);
	namedWindow("ʶ����ͼ", 1);//��������
	imshow("ʶ����ͼ", dst);
	//namedWindow("����", 1);//��������
	//imshow("����", pt);
	waitKey(0);
	return 0;
}

Mat TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src)
{

	//ѭ������
	int i, j, m, n;
	ofstream out("R.txt");
	double dSumT; //ģ��Ԫ�ص�ƽ����
	double dSumS; //ͼ��������Ԫ�ص�ƽ����
	double dSumST; //ͼ���������ģ��ĵ��	

	//��Ӧֵ
	double R;

	//��¼��ǰ�������Ӧ
	double MaxR;

	//�����Ӧ����λ��
	int nMaxX=0;
	int nMaxY=0;

	int nHeight = src->rows;
	int nWidth = src->cols;
	//ģ��ĸߡ���
	int nTplHeight = pTemplate->rows;
	int nTplWidth = pTemplate->cols;

	//���� dSumT
	dSumT = 0;
	for (m = 0; m < nTplHeight; m++)
	{
		for (n = 0; n < nTplWidth; n++)
		{
			// ģ��ͼ���m�У���n�����صĻҶ�ֵ
			int nGray = *pTemplate->ptr(m, n);

			dSumT += (double)nGray*nGray;
		}
	}

	//�ҵ�ͼ���������Ӧ�ĳ���λ��
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
					// ԭͼ���i+m�У���j+n�����صĻҶ�ֵ
					int nGraySrc = *src->ptr(i + m, j + n);

					// ģ��ͼ���m�У���n�����صĻҶ�ֵ
					int nGrayTpl = *pTemplate->ptr(m, n);

					dSumS += (double)nGraySrc*nGraySrc;
					dSumST += (double)nGraySrc*nGrayTpl;
				}
			}

			R = dSumST / (sqrt(dSumS)*sqrt(dSumT));//���������Ӧ
			out << R <<' ';

			//����������ԱȽ�
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