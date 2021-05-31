#include "cv.h"                         
#include "highgui.h"
#include "cvaux.h"
#include "cxcore.h"
#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;



//��������dividedLinearStrength

//���ã�ʵ�ֶַ���������

//������

//matInput������ͼ��

//matOutput : ���ͼ��

//fStart : �ֶ��������

//fEnd �� �ֶ������յ�

//fSout��ӳ���������

//fEout��ӳ�������յ�

//����ֵ����

//ע��֧�ֵ�ͨ��8λ�Ҷ�ͼ��

void dividedLinearStrength(cv::Mat& matInput, cv::Mat& matOutput, float fStart, float fEnd,

	float fSout, float fEout)

{

	//����ֱ�߲���

	//L1

	float fK1 = fSout / fStart;

	//L2

	float fK2 = (fEout - fSout) / (fEnd - fStart);

	float fC2 = fSout - fK2 * fStart;

	//L3

	float fK3 = (255.0f - fEout) / (255.0f - fEnd);

	float fC3 = 255.0f - fK3 * 255.0f;



	//������ѯ��

	std::vector<unsigned char> loolUpTable(256);

	for (size_t m = 0; m < 256; m++)

	{

		if (m < fStart)

		{

			loolUpTable[m] = static_cast<unsigned char>(m * fK1);

		}

		else if (m > fEnd)

		{

			loolUpTable[m] = static_cast<unsigned char>(m * fK3 + fC3);

		}

		else

		{

			loolUpTable[m] = static_cast<unsigned char>(m * fK2 + fC2);

		}

	}

	//�������ͼ��

	matOutput = cv::Mat::zeros(matInput.rows, matInput.cols, matInput.type());

	//�Ҷ�ӳ��

	for (size_t r = 0; r < matInput.rows; r++)

	{

		unsigned char* pInput = matInput.data + r * matInput.step[0];

		unsigned char* pOutput = matOutput.data + r * matOutput.step[0];

		for (size_t c = 0; c < matInput.cols; c++)

		{

			//���gamma�任

			pOutput[c] = loolUpTable[pInput[c]];

		}

	}

}


int main()

{

	cv::Mat matSrc = cv::imread("aerial.tif", cv::IMREAD_GRAYSCALE);

	cv::imshow("ԭʼͼ", matSrc);

	cv::Mat matDLS;

	dividedLinearStrength(matSrc, matDLS, 125, 250, 5, 240);

	cv::imshow("�ֶ����Ա任", matDLS);

	cv::waitKey(0);

	return 0;

}

