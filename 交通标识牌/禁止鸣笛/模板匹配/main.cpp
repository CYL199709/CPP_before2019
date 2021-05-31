
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2/video/video.hpp"

using namespace cv;
using namespace std;

//主函数
int main()
{
	Mat image_source = imread("../picture/src.bmp", 1);
	Mat image_template = imread("../picture/tempnew_coler.bmp", 1);

#if 0 //创建模板用的
	//Mat image_color1;
	//cvtColor(image_source, image_color1, CV_GRAY2BGR);
	//rectangle(image_color1, cvPoint(190, 145), cvPoint(290, 245), Scalar(255, 0, 0), 2, 1, 0);
	Mat Temp = image_source(Rect(190, 145, 100, 100)); 
	imwrite("tempnew_coler.bmp",Temp);
#endif

	Mat image_matched;
	matchTemplate(image_source, image_template, image_matched, TM_CCOEFF_NORMED);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	//寻找最佳匹配位置  
	minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);
	// 模板中心
	int template_x = maxLoc.x + image_template.cols / 2;
	int template_y = maxLoc.y + image_template.rows / 2;  //模板中心

	Mat image_dst;
	image_source.copyTo(image_dst);
	circle(image_dst, Point(template_x, template_y), 2, Scalar(0, 0, 255), 2, 8, 0);
	rectangle(image_dst, cvPoint(template_x - image_template.cols / 2, template_y - image_template.rows / 2), cvPoint(template_x + image_template.cols / 2, template_y + image_template.rows / 2), Scalar(255, 0, 0), 2, 1, 0);

	Mat Save = image_source(Rect(template_x - image_template.cols / 2, template_y - image_template.rows / 2, image_template.cols, image_template.rows)); //右边图前N列
	imwrite("save.bmp", Save);

	cout << "禁止鸣喇叭" << endl;

	namedWindow("原图", 1);//创建窗口
	imshow("原图", image_source);
	namedWindow("处理结果图", 1);//创建窗口
	imshow("处理结果图", image_dst);
	namedWindow("保存", 1);//创建窗口
	imshow("保存", Save);

	waitKey(0);

	return 0;
}
