#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


using namespace std;
using namespace cv;

Mat img; Mat templ; Mat result;
char* image_window = "result"; //窗口名称定义



int match_method;
int max_Trackbar = 5;




void MatchingMethod(int, void*)
{

	Mat img_display;
	img.copyTo(img_display); //将 img的内容拷贝到 img_display

	/// 创建输出结果的矩阵
	int result_cols = img.cols - templ.cols + 1;     //计算用于储存匹配结果的输出图像矩阵的大小。
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);//被创建矩阵的列数，行数，以CV_32FC1形式储存。

	/// 进行匹配和标准化
	matchTemplate(img, templ, result, match_method); //待匹配图像，模版图像，输出结果图像，匹配方法（由滑块数值给定。）
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());//输入数组，输出数组，range normalize的最小值，range normalize的最大值，归一化类型，当type为负数的时候输出的type和输入的type相同。

	/// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//用于检测矩阵中最大值和最小值的位置

	/// 对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0); //将得到的结果用矩形框起来
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow(image_window, img_display);//输出最终的到的结果
	

	return;
}


int main(int argc, char** argv)
{

	img = imread("../picture/1.jpg");//载入待匹配图像
	templ = imread("../picture/2.png");//载入模版图像

	/// 创建窗口
	namedWindow(image_window, CV_WINDOW_AUTOSIZE); // 窗口名称，窗口标识CV_WINDOW_AUTOSIZE是自动调整窗口大小以适应图片尺寸。
	

	
	MatchingMethod(0, 0);//初始化显示
	cout << "禁止调头" << endl;
	
	waitKey(0);
	return 0;
}