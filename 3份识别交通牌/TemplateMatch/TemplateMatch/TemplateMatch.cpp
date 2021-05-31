#include"TemplateMatch.h"

void TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src)
{

	//ѭ������
	int i, j, m, n;

	double dSumT; //ģ��Ԫ�ص�ƽ����
	double dSumS; //ͼ��������Ԫ�ص�ƽ����
	double dSumST; //ͼ���������ģ��ĵ��	

				   //��Ӧֵ
	double R;

	//��¼��ǰ�������Ӧ
	double MaxR;

	//�����Ӧ����λ��
	int nMaxX;
	int nMaxY;
	
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
			int nGray =*pTemplate->ptr(m, n);

			dSumT += (double)nGray*nGray;
		}
	}

	//�ҵ�ͼ���������Ӧ�ĳ���λ��
	MaxR = 0;
	for (i = 0; i < nHeight - nTplHeight + 1; i++)
	{
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

												   //����������ԱȽ�
			if (R > MaxR)
			{
				MaxR = R;
				nMaxX = j;
				nMaxY = i;
			}
		}
	}

						  //���ҵ������ƥ�������Ƶ�Ŀ��ͼ��
	for (m = 0; m < nTplHeight; m++)
	{
		for (n = 0; n < nTplWidth; n++)
		{
			int nGray = *src->ptr(nMaxY + m, nMaxX + n);
			//pTo->setTo(nMaxX + n, nMaxY + m, RGB(nGray, nGray, nGray));
			pTo->at<BYTE>(nMaxY + m, nMaxX + n) = nGray;
		}
	}

}