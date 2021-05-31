#include"TemplateMatch.h"
#include<iostream>

using namespace std;
int main()
{
	Mat src = imread("./src3.jpg", 0);
	Mat Template = imread("./template3.jpg", 0);
	Mat pt=src;
	pt.data = new BYTE[src.cols*src.rows];
	memset(pt.data, 255, src.cols*src.rows);
	TemplateMatch(&pt, &Template, &src);
	imshow("S", src);
	imshow("T", Template);
	imshow("P", pt);

	imwrite("S.jpg", src);
	imwrite("T.jpg", Template);
	imwrite("P.jpg", pt);
	waitKey(0);
	return 0;
}