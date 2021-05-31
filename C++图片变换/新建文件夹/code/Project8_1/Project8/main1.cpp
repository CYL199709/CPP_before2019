#include<iostream>
#include<highgui\highgui.hpp>
#include<core/core.hpp>  
#include<math.h>
using namespace cv;
using namespace std;
// get Gamma transformation look up table
void GetGammaTransLUT(uchar *pLUT, float Gamma, int iLUTLen)
{
	for (int i = 0; i<iLUTLen; i++)
	{
		pLUT[i] = (uchar)(pow((float)i / 255.0, Gamma) * 255);
	}
}
void GammaTrans(uchar *pSrc, uchar *pDst, const int iHeight,
	const int iWidth, float Gamma)
{
	uchar *pLUT = new uchar[256];
	GetGammaTransLUT(pLUT, Gamma, 256);
	for (int i = 0; i<iHeight*iWidth; i++)
	{
		pDst[i] = (uchar)pLUT[pSrc[i]];
	}
	delete[]pLUT;
}
int main()
{
	Mat image = imread("mars.tif", 0);
	Mat image_Dst = imread("mars.tif", 0);
	const int iHeight = image.rows;
	const int iWidth = image.cols;
	uchar* pSrc = image.data;//new uchar[iHeight*iWidth];
	uchar* pDst = image_Dst.data;//new uchar[iHeight*iWidth];
	GammaTrans(pSrc, pDst, iHeight, iWidth, 0.6);
	//namedWindow("Origin",1);
	imshow("Origin", image);
	 
	//ÏÔÊ¾Í¼Ïñ  
	imshow("Gamma Trans", image_Dst);
	//µÈ´ý°´¼ü  
	waitKey();
	return 0;
}
