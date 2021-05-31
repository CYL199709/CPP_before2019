#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


using namespace std;
using namespace cv;

Mat img; Mat templ; Mat result;
Mat temp2;
char* image_window = "result"; //�������ƶ���

int match_method;
int max_Trackbar = 5;


void MatchingMethod(int, void*)
{

	Mat img_display;
	img.copyTo(img_display); //�� img�����ݿ����� img_display

	/// �����������ľ���
	int result_cols = img.cols - templ.cols + 1;     //�������ڴ���ƥ���������ͼ�����Ĵ�С��
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);//�������������������������CV_32FC1��ʽ���档
	match_method = CV_TM_SQDIFF_NORMED;
	/// ����ƥ��ͱ�׼��
	matchTemplate(img, templ, result, match_method); //��ƥ��ͼ��ģ��ͼ��������ͼ��ƥ�䷽�����ɻ�����ֵ��������
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());//�������飬������飬range normalize����Сֵ��range normalize�����ֵ����һ�����ͣ���typeΪ������ʱ�������type�������type��ͬ��

	/// ͨ������ minMaxLoc ��λ��ƥ���λ��
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//���ڼ����������ֵ����Сֵ��λ��

	/// ���ڷ��� SQDIFF �� SQDIFF_NORMED, ԽС����ֵ������ߵ�ƥ����. ��������������, ��ֵԽ��ƥ��Խ��
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	matchLoc.x = matchLoc.x - 40;
	matchLoc.y = matchLoc.y - 40;
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols+80, matchLoc.y + templ.rows+80), Scalar(0, 0, 255), 2, 8, 0); //���õ��Ľ���þ��ο�����
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow(image_window, img_display);//������յĵ��Ľ��
	return;
}


int main(int argc, char** argv)
{

	img = imread("../picture/1.jpg");//�����ƥ��ͼ��
	temp2 = imread("../picture/3.jpg");//����ģ��ͼ��

	/// ��������
	namedWindow(image_window, CV_WINDOW_AUTOSIZE); // �������ƣ����ڱ�ʶCV_WINDOW_AUTOSIZE���Զ��������ڴ�С����ӦͼƬ�ߴ硣

	

	templ = temp2(Range(40, 100), Range(40, 100));

	

	cv::GaussianBlur(img, img, cv::Size(5, 5), 1.5);
	
	MatchingMethod(0, 0);//��ʼ����ʾ
	cout << "��ֹ��ͷ" << endl;
	
	waitKey(0);
	return 0;
}