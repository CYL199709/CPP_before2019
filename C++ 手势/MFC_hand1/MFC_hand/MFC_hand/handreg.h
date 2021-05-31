#ifndef HAND_H
#define HAND_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include<vector>
#include<iostream>
#include<math.h>
#include <thread>
//#include<Windows.h>
//#include<Winuser.h>

using namespace cv;
using namespace std;

class Myhand
{
public:
	Mat srcimage;//ԭͼ��
	Mat dstimage;//������ͼ��

	Point  face_center1 = { 200, 200 }, face_center2 = { 300, 300 };//��������
	Rect rt = {300, 300, 50, 50}; //�������δ�С
	int face_num = 0;//���ĸ���
	
	Point2f hand_center; //�ֲ�����
	Point fingerTips_single;//ָ������
	int finger_find_num;
	Point2f mc;//������������



	String face_cascade_name;
	CascadeClassifier face_cascade;
	Myhand(string ca)
	{
		face_cascade_name = ca;
	}

	//����ָ��
	//src :Ϊ��ֵ��ͼ��ֻ���ֲ�����
	//max_contours :Ϊsrc�еģ����ģ����������ֲ�����
	//center :Ϊ�ֲ�����������
	//����ָ�����
	void gethandpoint(Mat src, vector<Point> max_contours, Point center);


	//����������ͨ������
	//src :Ϊ��ֵ��ͼ
	//dst :Ϊ���ָ��λ�ú�Ķ�ֵ��ͼ
	vector<Point> Maxarea_find(Mat src, Mat dst);

	int mymain();

	//�������
	// frame ԭ��ɫͼ��
	// ���� faces.size() ����������
	void detect_face(Mat frame);

	////������������߳����ú���
	//static  void  t_detect_face(Myhand &img);

private:
	//���otsu
	// src ��ͨ��ͼ��
	// dst ��ֵ��ͼ
	void cvThresholdOtsu(Mat src, Mat dst);

	//ʹ��Crͨ�����зָ��ȥ������
	//src:Ϊ��ɫͼ
	//rt :Ϊ��������ľ������Ͻ�����ε�with��hight
	//face_num :Ϊsrcͼ�е���������
	//���أ�������ͼ��ȥ��������Ķ�ֵ��ͼ
	Mat cvSkinOtsu_YCrCb(Mat src, Rect rt, int face_num);//yCbCr

	//�������������ĵ����ߵļнǵĺ���
	//a = (first_p.x, first_p.y)Ϊ��һ������
	//b = (second_p.x, second_p.y)Ϊ�ڶ�������
	double getAngle_center(Point first_p, Point second_p);

};

#endif