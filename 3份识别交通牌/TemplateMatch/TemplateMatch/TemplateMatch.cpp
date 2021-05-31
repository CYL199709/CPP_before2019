#include"TemplateMatch.h"

void TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src)
{

	//循环变量
	int i, j, m, n;

	double dSumT; //模板元素的平方和
	double dSumS; //图像子区域元素的平方和
	double dSumST; //图像子区域和模板的点积	

				   //响应值
	double R;

	//记录当前的最大响应
	double MaxR;

	//最大响应出现位置
	int nMaxX;
	int nMaxY;
	
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
			int nGray =*pTemplate->ptr(m, n);

			dSumT += (double)nGray*nGray;
		}
	}

	//找到图像中最大响应的出现位置
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
					// 原图像第i+m行，第j+n列象素的灰度值
					int nGraySrc = *src->ptr(i + m, j + n);

					// 模板图像第m行，第n个象素的灰度值
					int nGrayTpl = *pTemplate->ptr(m, n);

					dSumS += (double)nGraySrc*nGraySrc;
					dSumST += (double)nGraySrc*nGrayTpl;
				}
			}

			R = dSumST / (sqrt(dSumS)*sqrt(dSumT));//计算相关响应

												   //与最大相似性比较
			if (R > MaxR)
			{
				MaxR = R;
				nMaxX = j;
				nMaxY = i;
			}
		}
	}

						  //将找到的最佳匹配区域复制到目标图像
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